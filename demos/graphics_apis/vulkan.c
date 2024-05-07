/*
============================================================
                        DEMO INFO

DEMO NAME:          vulkan.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-27
LAST UPDATED:       2024-05-03

============================================================
                        DEMO PURPOSE

This demo shows Vulkan working in muCOSA.

============================================================
                        LICENSE INFO

All code is licensed under MIT License or public domain, 
whichever you prefer.
More explicit license information at the end of file.

============================================================
                       COMPILER NOTES

vkbind does violate function declarations without
prototypes, so it needs to be turned off when compiling this
demo when relevant.

============================================================
                        ATTRIBUTIONS

This demo was produced primarily with these resources:
https://github.com/mackron/vkbind/
https://registry.khronos.org/vulkan/specs/1.3/html/vkspec.html
https://vulkan-tutorial.com/
https://www.youtube.com/watch?v=e14z9oOsPu0
https://github.com/mackron/vkbind/tree/master/examples/01_Fundamentals

============================================================

@TODO Add more explanatory comments
@TODO Handle window resizing (actually harder than you'd think, tanks for nothing, X11!)
*/

// Include muCOSA

#define VKBIND_IMPLEMENTATION // For when vkbind is included by muCOSA
#define MUCOSA_NAMES // For name functions
#define MUCOSA_VULKAN // For Vulkan support
#define MUCOSA_VULKAN_INCLUDE_PATH <vkbind.h> // For muCOSA to include Vulkan (needed for certain macros)
#define MUCOSA_IMPLEMENTATION
#include "muCOSA.h"

#include <stdio.h> // For printf
#include <stdlib.h> // For allocation
#include <string.h> // For strcmp
#include <stdint.h> // For UINT32_MAX and UINT64_MAX

/* Global variables */
	
	// For storing the result of muCOSA functions
	muCOSAResult res = MUCOSA_SUCCESS;
	// "safe call"; to automatically print if the function didn't go well
	#define scall(func) if (res != MUCOSA_SUCCESS) { printf("[WARNING] '" #func "' returned %s\n", muCOSA_result_get_name(res)); }

	// Window
	muWindow win = MU_NONE;

	// Initial window width/height
	uint32_m win_width = 800, win_height = 600;

/* Vulkan setup */

	muBool vk_init(void);

	/* Initiation stuff */

	// Instance, which wraps the entire Vulkan context.
	VkInstance instance = VK_NULL_HANDLE;
	// Boolean for whether or not to print Vulkan debug messages. If you don't have the Vulkan API
	// installed on your system, it's likely Vulkan debug messaging won't be available, so this
	// will automatically be set to MU_FALSE when that's figured out, but setting this to MU_FALSE
	// initially will also prevent it from even checking.
	muBool use_validation_layers = MU_TRUE;
	muBool vk_create_instance(void);
	muBool vk_destroy_instance(void);

	// Validation layer, which can print debug messages if available.
	VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
	muBool vk_create_debug_messenger(void);
	muBool vk_destroy_debug_messenger(void);

	// Surface, which is used to render onto the window; operating system dependent, so muCOSA will
	// do most of the heavy lifting for us.
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	muBool vk_create_surface(void);
	muBool vk_destroy_surface(void);

	// Physical device, which exists as a hardware implementation of the Vulkan API.
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	muBool vk_create_physical_device(void);
	muBool vk_destroy_physical_device(void);

	// Logical device, aka "device", which exists as a logical interface to the physical device, to
	// which we'll use to hold queues to send instructions to the GPU.
	VkDevice device = VK_NULL_HANDLE;
	// Queue family that holds graphics functionality, as well as the queue created from it that
	// will actually be used to send requests.
	uint32_t graphics_family;
	VkQueue graphics_queue;
	// Presentation family/queue that will hold the ability to present things to the surface.
	uint32_t present_family;
	VkQueue present_queue;
	muBool vk_create_device(void);
	muBool vk_destroy_device(void);

	/* Rendering stuff */

	// Swapchain, which represents the images that are being buffered and written to while the
	// program is running.
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	// Format of the swapchain, which we're storing for usage in color attachment descriptors in
	// objects such as the render pass, which need to know exactly how the data of the swapchain is
	// formatted.
	VkFormat swapchain_format = MU_ZERO_STRUCT_CONST(VkFormat);
	// Dimensions of the swapchain, which need to be stored since other objects like framebuffers
	// need to know info about the dimensions as well.
	VkExtent2D swapchain_extent = MU_ZERO_STRUCT_CONST(VkExtent2D);
	// Swapchain viewport and scissor, which is just more info about the dimensions that other
	// parts of the code need later. (These are actually initialized in the pipeline but shhhh)
	VkViewport swapchain_viewport = MU_ZERO_STRUCT_CONST(VkViewport);
	VkRect2D swapchain_scissor = MU_ZERO_STRUCT_CONST(VkRect2D);
	// The swapchain images themselves.
	uint32_t swapchain_image_count;
	VkImage* swapchain_images = 0;
	// The image views of the swapchain that describe the image data. (count is
	// swapchain_image_count)
	VkImageView* swapchain_image_views = 0;
	// The index of what swapchain image is being used, as it will loop through the images frame by
	// frame.
	uint32_t swapchain_image_index = 0;
	muBool vk_create_swapchain(void);
	muBool vk_destroy_swapchain(void);

	// Command pools, which are used to store commands that a command buffer will eventually
	// execute.
	VkCommandPool command_pool = VK_NULL_HANDLE;
	// Command buffers, which are used to, as their name would imply, buffer commands, allowing us
	// to send a LOT of graphics rendering tasks in one big buffer, which we can then send to a
	// graphics queue, which then allows the GPU to handle all of the tasks and make optimizations
	// as it sees fit.
	VkCommandBuffer command_buffer = VK_NULL_HANDLE;
	// Semaphores, which are used to make one queue operation wait on another. In this case,
	// queue_wait_semaphore is used to wait for the next swapchain image to be available for
	// rendering, set up in the call to vkAcquireNextImageKHR and waited upon in the call to
	// vkQueueSubmit, and queue_signal_semaphore is used to wait for the draw commands to complete,
	// set up for signaling in vkQueueSubmit and waited upon in the call to vkQueuePresentKHR.
	VkSemaphore queue_wait_semaphore = VK_NULL_HANDLE;
	VkSemaphore queue_signal_semaphore = VK_NULL_HANDLE;
	// A fence, which is used to wait on the GPU to finish before we, the CPU, do something (unlike
	// the semaphore, which is used to order events on the GPU). In this case, we're using this to
	// wait for the graphics queue to be ready to be sent commands. This is a *little* redundant,
	// since queue_signal_semaphore is used to wait for the commands to have finished, but
	// technically speaking, the time when all of the commands sent to a queue have finished and
	// when exactly the queue is ready to be sent commands again are slightly different.
	VkFence queue_wait_fence = VK_NULL_HANDLE;
	muBool vk_create_command_buffer(void);
	muBool vk_destroy_command_buffer(void);

	// Shader code, which is stored in raw SPIR-V bytecode at the bottom of the file, since there's
	// no convenient and transportable way to compile GLSL to SPIR-V in this context.
	#define VERTEX_CODE_BYTES 1500
	extern muByte vertex_code[VERTEX_CODE_BYTES];
	#define FRAGMENT_CODE_BYTES 564
	extern muByte fragment_code[FRAGMENT_CODE_BYTES];
	// Shader modules, which store our shaders in Vulkan
	VkShaderModule vertex_module = VK_NULL_HANDLE;
	VkShaderModule fragment_module = VK_NULL_HANDLE;
	muBool vk_create_shader_modules(void);
	muBool vk_destroy_shader_modules(void);

	// The render pass, which describes the image resources used during rendering, as well as how
	// they will be used, which, in our case, is just the actual images we're rendering to.
	VkRenderPass render_pass = VK_NULL_HANDLE;
	muBool vk_create_render_pass(void);
	muBool vk_destroy_render_pass(void);
	// The framebuffer essentially wraps the render pass and the attachments together, which, in
	// our case, is just the swapchain image to be presented. We're holding one framebuffer for
	// each swapchain image.
	VkFramebuffer* framebuffers = 0;
	muBool vk_create_framebuffers(void);
	muBool vk_destroy_framebuffers(void);

	// Pipeline layout, which is used for passing data to the shaders, which we're not doing.
	VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
	// Graphics pipeline, which essentially describes a render state and how the vertex data and
	// shaders will be processed and put to the screen
	VkPipeline pipeline = VK_NULL_HANDLE;
	muBool vk_create_pipeline(void);
	muBool vk_destroy_pipeline(void);

	muBool vk_term(void);

/* Vulkan rendering */

	// This function makes and passes the entire render pass, getting the GPU to work on drawing
	// the image.
	muBool vk_render(void);

	// This function presents the rendered image onto screen.
	muBool vk_present(void);

int main(void) {
/* Initiation */

	// Initiate muCOSA
	muCOSA_init(&res, MU_WINDOW_SYSTEM_AUTO); scall(muCOSA_init)

	// Create window to be rendered on
	win = mu_window_create(&res, 
		MU_NO_GRAPHICS_API, 0, 
		(muByte*)"Vulkan demo",
		win_width, win_height,
		mu_window_default_create_info()
	); scall(mu_window_create)

/* Vulkan initialization */

	if (!vk_init()) {
		muCOSA_term(0);
		return -1;
	}

/* Main loop */

	while (!mu_window_get_closed(&res, win)) { scall(mu_window_get_closed)
		// Send all render calls
		vk_render();

		// Present finalized image to screen
		vk_present();

		// Note how we don't need to swap buffers now; that's being handled by us.

		// Update window
		mu_window_update(&res, win); scall(mu_window_update)
	}
	scall(mu_window_get_closed)

/* Termination */

	// Due to X11/Vulkan being weird, on X11 requires us to destory Vulkan stuff AFTER its given
	// window is fully destroyed.
	// https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers/issues/1894

	// Destroy Vulkan if non-X11
	muWindowSystem window_system = muCOSA_get_current_window_system(&res); scall(muCOSA_get_current_window_system)
	if (window_system != MU_WINDOW_SYSTEM_X11) {
		vk_term();
	} else {
		// X11 *does* want the swapchain to be destroyed here, though
		vkDestroySwapchainKHR(device, swapchain, 0);
		swapchain = VK_NULL_HANDLE;
	}

	// Destroy window
	mu_window_destroy(&res, win); scall(mu_window_destroy)

	// Destroy Vulkan if X11
	if (window_system == MU_WINDOW_SYSTEM_X11) {
		vk_term();
	}

	// Terminate muCOSA
	muCOSA_term(&res); scall(muCOSA_term)

	return 0;
}

/* Vulkan rendering */
	
	muBool vk_render(void) {
		/* Start getting swapchain image */

		if (vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, queue_wait_semaphore, VK_NULL_HANDLE, &swapchain_image_index) != VK_SUCCESS) {
			printf("[WARNING] Failed to get the next swapchain image\n");
			return MU_FALSE;
		}

		/* Wait for the graphics queue to be ready to be sent commands again */

		if (vkWaitForFences(device, 1, &queue_wait_fence, VK_TRUE, UINT64_MAX) != VK_SUCCESS) {
			printf("[WARNING] Failed to wait for queue-wait fence\n");
			return MU_FALSE;
		}

		if (vkResetFences(device, 1, &queue_wait_fence) != VK_SUCCESS) {
			printf("[WARNING] Failed to reset queue-wait fence\n");
			return MU_FALSE;
		}

		/* Reset and start the command buffer, which will store all our instructions */

		if (vkResetCommandBuffer(command_buffer, 0) != VK_SUCCESS) {
			printf("[WARNING] Failed to reset command buffer\n");
			return MU_FALSE;
		}

		VkCommandBufferBeginInfo cb_ci = MU_ZERO_STRUCT(VkCommandBufferBeginInfo);
		cb_ci.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(command_buffer, &cb_ci) != VK_SUCCESS) {
			printf("[WARNING] Failed to begin command buffer\n");
			return MU_FALSE;
		}

		/* Start the render pass, which will begin our instructions */

		VkRenderPassBeginInfo rp_ci = MU_ZERO_STRUCT(VkRenderPassBeginInfo);
		rp_ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		rp_ci.renderPass = render_pass;
		rp_ci.framebuffer = framebuffers[swapchain_image_index];
		rp_ci.renderArea.offset.x = 0;
		rp_ci.renderArea.offset.y = 0;
		rp_ci.renderArea.extent = swapchain_extent;

		VkClearValue clear_value = MU_ZERO_STRUCT(VkClearValue);
		clear_value.color.float32[0] = clear_value.color.float32[1] = clear_value.color.float32[2] = clear_value.color.float32[3] = 1.f;

		rp_ci.clearValueCount = 1;
		rp_ci.pClearValues = &clear_value;

		vkCmdBeginRenderPass(command_buffer, &rp_ci, VK_SUBPASS_CONTENTS_INLINE);

		/* Bind and draw our pipeline; this is our instruction to the command buffer */

		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		// (We also need to bind dynamic state)

		vkCmdSetScissor(command_buffer, 0, 1, &swapchain_scissor);
		vkCmdSetViewport(command_buffer, 0, 1, &swapchain_viewport);

		vkCmdDraw(command_buffer, 3, 1, 0, 0);

		/* End our render pass */

		vkCmdEndRenderPass(command_buffer);

		/* Stop the command buffer, wrapping up all our instructions */

		if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
			printf("[WARNING] Failed to end command buffer\n");
			return MU_FALSE;
		}

		/* Submit everything to the graphics queue, starting the rendering */

		// (For wait semaphore, wait until all commands relevant to the swapchain image being
		// acquired have finished, ensuring that the image is fully available before the queued
		// commands start executing.)
		VkPipelineStageFlags wait_dst = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		VkSubmitInfo submit_info = MU_ZERO_STRUCT(VkSubmitInfo);
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = &queue_wait_semaphore;
		submit_info.pWaitDstStageMask = &wait_dst;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = &queue_signal_semaphore;
		// ^ This sets up queue_signal_semaphore to be signaled when all of our commands are
		// submitted to the graphics queue, which we wait for in the present function.

		if (vkQueueSubmit(graphics_queue, 1, &submit_info, queue_wait_fence) != VK_SUCCESS) {
			printf("[WARNING] Failed to submit commands to graphics queue\n");
			return MU_FALSE;
		}

		return MU_TRUE;
	}

	muBool vk_present(void) {
		/* Present current swapchain, when commands are finished of course */

		VkPresentInfoKHR p_i = MU_ZERO_STRUCT(VkPresentInfoKHR);
		p_i.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		p_i.waitSemaphoreCount = 1;
		p_i.pWaitSemaphores = &queue_signal_semaphore;
		p_i.swapchainCount = 1;
		p_i.pSwapchains = &swapchain;
		p_i.pImageIndices = &swapchain_image_index;

		if (vkQueuePresentKHR(present_queue, &p_i) != VK_SUCCESS) {
			printf("[WARNING] Failed to present to the screen\n");
			return MU_FALSE;
		}

		return MU_TRUE;
	}

/* Vulkan functions */

	/* Initiation */

		muBool vk_init(void) {
			if (vkbInit(NULL) != VK_SUCCESS) {
				printf("[WARNING] Call to vkbInit failed.\n");
				return MU_FALSE;
			}

			if (!vk_create_instance()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_debug_messenger()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_surface()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_physical_device()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_device()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_swapchain()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_command_buffer()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_shader_modules()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_render_pass()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_framebuffers()) {
				vk_term();
				return MU_FALSE;
			}
			if (!vk_create_pipeline()) {
				vk_term();
				return MU_FALSE;
			}

			return MU_TRUE;
		}

	/* Instance */

		#define VALIDATION_LAYER_COUNT 1
		const char* validation_layers[] = {
			"VK_LAYER_KHRONOS_validation"
		};

		muBool vk_are_validation_layers_available(void) {
			if (!use_validation_layers) {
				return MU_FALSE;
			}

			uint32_t prop_count = 0;
			if (vkEnumerateInstanceLayerProperties(&prop_count, 0) != VK_SUCCESS) {
				printf("[DEBUG] Failed to get instance layer property count\n");
				use_validation_layers = MU_FALSE;
				return MU_FALSE;
			}
			if (prop_count == 0) {
				printf("[DEBUG] No instance layer properties found\n");
				use_validation_layers = MU_FALSE;
				return MU_FALSE;
			}

			VkLayerProperties* props = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * prop_count);
			if (props == 0) {
				printf("[DEBUG] Failed to allocate memory for layer properties\n");
				use_validation_layers = MU_FALSE;
				return MU_FALSE;
			}
			if (vkEnumerateInstanceLayerProperties(&prop_count, props) != VK_SUCCESS) {
				printf("[DEBUG] Failed to enumerate instance layer properties\n");
				free(props);
				use_validation_layers = MU_FALSE;
				return MU_FALSE;
			}

			for (uint32_t i = 0; i < VALIDATION_LAYER_COUNT; i++) {
				muBool found = MU_FALSE;
				for (uint32_t j = 0; j < prop_count; j++) {
					if (strcmp(props[j].layerName, validation_layers[i]) == 0) {
						found = MU_TRUE;
						break;
					}
				}

				if (!found) {
					printf("[DEBUG] Validation layer \"%s\" unavailable; Vulkan debug messages will not be able to appear\n", validation_layers[i]);
					use_validation_layers = MU_FALSE;
					free(props);
					return MU_FALSE;
				}
			}

			free(props);
			return MU_TRUE;
		}
	
		muBool vk_create_instance(void) {
			/* Instance */

			VkInstanceCreateInfo cinfo = MU_ZERO_STRUCT(VkInstanceCreateInfo);
			cinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

			size_m surface_extension_count = 0;
			const char** surface_extensions = mu_vulkan_get_surface_instance_extensions(&res, &surface_extension_count);
			scall(mu_vulkan_get_surface_instance_extensions)

			const char** actual_surface_extensions = surface_extensions;
			if (vk_are_validation_layers_available()) {
				cinfo.enabledLayerCount = VALIDATION_LAYER_COUNT;
				cinfo.ppEnabledLayerNames = (const char* const*)validation_layers;

				size_m surface_extension_size = 0;
				for (size_m i = 0; i < surface_extension_count; i++) {
					surface_extension_size += strlen(surface_extensions[i]) + 1;
				}
				size_m validation_layer_size = strlen(VK_EXT_DEBUG_UTILS_EXTENSION_NAME) + 1;
				actual_surface_extensions = (const char**)malloc(surface_extension_size+validation_layer_size);
				if (actual_surface_extensions == 0) {
					printf("[WARNING] Failed to allocate memory for surface extensions\n");
					return MU_FALSE;
				}

				memcpy(actual_surface_extensions, surface_extensions, surface_extension_size);
				actual_surface_extensions[surface_extension_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
				surface_extension_count += 1;
			}

			cinfo.enabledExtensionCount = (uint32_t)surface_extension_count;
			cinfo.ppEnabledExtensionNames = (const char* const*)actual_surface_extensions;

			if (vkCreateInstance(&cinfo, 0, &instance) != VK_SUCCESS) {
				printf("[WARNING] Call to vkCreateInstance failed.\n");
				if (actual_surface_extensions != surface_extensions) {
					free(actual_surface_extensions);
				}
				return MU_FALSE;
			}

			if (actual_surface_extensions != surface_extensions) {
				free(actual_surface_extensions);
			}

			/* Possible validation layers */
			// (Note: failures here aren't fatal)

			if (!use_validation_layers) {
				return MU_TRUE;
			}

			uint32_t prop_count = 0;
			if (vkEnumerateInstanceLayerProperties(&prop_count, 0) != VK_SUCCESS) {
				printf("[DEBUG] Failed to get instance layer property count\n");
				return MU_TRUE;
			}
			if (prop_count == 0) {
				printf("[DEBUG] No instance layer properties found\n");
				return MU_TRUE;
			}

			VkLayerProperties* props = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * prop_count);
			if (props == 0) {
				printf("[DEBUG] Failed to allocate memory for layer properties\n");
				return MU_TRUE;
			}
			if (vkEnumerateInstanceLayerProperties(&prop_count, props) != VK_SUCCESS) {
				printf("[DEBUG] Failed to enumerate instance layer properties\n");
				free(props);
				return MU_TRUE;
			}

			for (uint32_t i = 0; i < VALIDATION_LAYER_COUNT; i++) {
				muBool found = MU_FALSE;
				for (uint32_t j = 0; j < prop_count; j++) {
					if (strcmp(props[j].layerName, validation_layers[i]) == 0) {
						found = MU_TRUE;
						break;
					}
				}

				if (!found) {
					printf("[DEBUG] Validation layer \"%s\" unavailable; Vulkan debug messages will not be able to appear\n", validation_layers[i]);
					free(props);
					return MU_TRUE;
				}
			}

			free(props);
			return MU_TRUE;
		}

		muBool vk_destroy_instance(void) {
			if (instance != VK_NULL_HANDLE) {
				vkDestroyInstance(instance, 0);
			}

			return MU_TRUE;
		}

	/* Debug messenger */

		VkResult vk_CreateDebugUtilsMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* p_create_info, const VkAllocationCallbacks* p_allocator, VkDebugUtilsMessengerEXT* p_debug_messenger) {
			PFN_vkCreateDebugUtilsMessengerEXT fun = (PFN_vkCreateDebugUtilsMessengerEXT)
			vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (fun != 0) {
				return fun(instance, p_create_info, p_allocator, p_debug_messenger);
			} else {
				printf("[WARNING] Failed to find function 'vkCreateDebugUtilsMessengerEXT'\n");
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		void vk_DestroyDebugUtilsMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* p_allocator) {
			PFN_vkDestroyDebugUtilsMessengerEXT fun = (PFN_vkDestroyDebugUtilsMessengerEXT)
			vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (fun != 0) {
				fun(instance, debug_messenger, p_allocator);
			} else {
				printf("[WARNING] Failed to find function 'vkDestroyDebugUtilsMessengerEXT'\n");
			}
		}

		unsigned int vk_DebugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT types, const VkDebugUtilsMessengerCallbackDataEXT* data, void* user_data) {
			if (user_data) {}

			printf("[DEBUG] (Vulkan debug message) ");
			
			printf("(severity: ");
			if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
				printf("verbose ");
			}
			if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
				printf("info ");
			}
			if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
				printf("warning ");
			}
			if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
				printf("error ");
			}
			printf(") ");

			printf("(types: ");
			if (types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
				printf("general ");
			}
			if (types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
				printf("validation ");
			}
			if (types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
				printf("performance ");
			}
			printf(") ");

			printf("message: \n%s\n", data->pMessage);
			return VK_TRUE;
		}

		muBool vk_create_debug_messenger(void) {
			if (!use_validation_layers) {
				return MU_TRUE;
			}

			VkDebugUtilsMessengerCreateInfoEXT ci = MU_ZERO_STRUCT(VkDebugUtilsMessengerCreateInfoEXT);
			ci.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			ci.messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | */
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			ci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			ci.pfnUserCallback = vk_DebugUtilsMessengerCallback;

			if (vk_CreateDebugUtilsMessenger(instance, &ci, 0, &debug_messenger) != VK_SUCCESS) {
				printf("[WARNING] Failed to create debug messenger\n");
				return MU_FALSE;
			}

			return MU_TRUE;
		}

		muBool vk_destroy_debug_messenger(void) {
			if (debug_messenger != VK_NULL_HANDLE) {
				vk_DestroyDebugUtilsMessenger(instance, debug_messenger, 0);
			}
			return MU_TRUE;
		}

	/* Surface */

		muBool vk_create_surface(void) {
			VkResult vkres = VK_SUCCESS;
			mu_vulkan_create_window_surface(&res, win,
				(void**)(&vkres),
				(void*)(&instance),
				(void**)(0),
				(void**)(&surface)
			); scall(mu_vulkan_create_window_surface)

			if (res != MUCOSA_SUCCESS || vkres != VK_SUCCESS) {
				printf("[WARNING] Failed to create window surface\n");
				return MU_FALSE;
			}

			return MU_TRUE;
		}

		muBool vk_destroy_surface(void) {
			if (surface != VK_NULL_HANDLE) {
				vkDestroySurfaceKHR(instance, surface, 0);
			}

			return MU_TRUE;
		}

	/* Physical device */

		// Extension needed for swapchain
		#define EXTENSION_COUNT 1
		const char* extensions[EXTENSION_COUNT] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		int32_m vk_rate_physical_device(VkPhysicalDeviceProperties props, VkPhysicalDeviceFeatures features,
			VkExtensionProperties* ext_props, size_m ext_prop_count) {
			int32_m score = 0;

			for (size_m i = 0; i < EXTENSION_COUNT; i++) {
				muBool found = MU_FALSE;
				for (size_m j = 0; j < ext_prop_count; j++) {
					if (strcmp(ext_props[j].extensionName, extensions[i]) == 0) {
						found = MU_TRUE;
						break;
					}
				}
				if (!found) {
					printf("[DEBUG] Physical device \"%s\" unsuitable; couldn't find extension \"%s\"\n", 
						props.deviceName, extensions[i]
					);
					return 0;
				}
			}

			switch (props.deviceType) {
				default: break;
				case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:   score += 500; break;
				case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score += 400; break;
				case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:    score += 300; break;
				case VK_PHYSICAL_DEVICE_TYPE_CPU:            score += 200; break;
			}

			if (features.fullDrawIndexUint32) score += 20;
			if (features.imageCubeArray) score += 30;
			if (features.geometryShader) score += 50;
			if (features.tessellationShader) score += 100;
			if (features.sampleRateShading) score += 80;
			if (features.multiViewport) score += 50;
			if (features.samplerAnisotropy) score += 60;
			if (features.shaderStorageImageMultisample) score += 50;

			printf("[DEBUG] Physical device \"%s\" rated %i\n", props.deviceName, (int)score);

			return score;
		}

		muBool vk_create_physical_device(void) {
			uint32_t physical_device_count = 0;
			if (vkEnumeratePhysicalDevices(instance, &physical_device_count, NULL) != VK_SUCCESS || physical_device_count == 0) {
				printf("[WARNING] Failed to find any physical devices\n");
				return MU_FALSE;
			}

			VkPhysicalDevice* physical_devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * physical_device_count);
			if (physical_devices == 0) {
				printf("[WARNING] Failed to allocate physical device list memory\n");
				return MU_FALSE;
			}
			if (vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices) != VK_SUCCESS) {
				printf("[WARNING] Failed to enumerate physical devices\n");
				free(physical_devices);
				return MU_FALSE;
			}

			int32_m best_score = 0;
			for (size_m i = 0; i < physical_device_count; i++) {
				VkPhysicalDeviceProperties props;
				vkGetPhysicalDeviceProperties(physical_devices[i], &props);
				VkPhysicalDeviceFeatures features;
				vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

				uint32_t extension_count = 0;
				vkEnumerateDeviceExtensionProperties(physical_devices[i], 0, &extension_count, 0);
				VkExtensionProperties* ext_props = 0;
				if (extension_count != 0) {
					ext_props = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * extension_count);
					if (ext_props == 0) {
						printf("[WARNING] Failed to allocate extension properties for physical device\n");
						extension_count = 0;
					} else {
						vkEnumerateDeviceExtensionProperties(physical_devices[i], 0, &extension_count, ext_props);
					}
				}

				int32_m score = vk_rate_physical_device(props, features, ext_props, extension_count);
				if (score > best_score) {
					best_score = score;
					physical_device = physical_devices[i];
				}

				if (ext_props != 0) {
					free(ext_props);
				}
			}

			if (best_score == 0) {
				printf("[WARNING] No useable physical device could be found\n");
				free(physical_devices);
				return MU_FALSE;
			}

			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(physical_device, &props);
			printf("[DEBUG] Physical device \"%s\" chosen\n", props.deviceName);

			free(physical_devices);
			return MU_TRUE;
		}

		muBool vk_destroy_physical_device(void) {
			// Auto destroyed, we musn't worry
			return MU_TRUE;
		}

	/* Device */

		muBool vk_create_device(void) {
			/* Queue families */

			uint32_t queue_family_property_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, 0);
			if (queue_family_property_count == 0) {
				printf("[WARNING] Failed to find queue family properties for physical device\n");
				return MU_FALSE;
			}

			VkQueueFamilyProperties* queue_family_properties = (VkQueueFamilyProperties*)malloc(
				sizeof(VkQueueFamilyProperties) * queue_family_property_count
			);
			if (queue_family_properties == 0) {
				printf("[WARNING] Failed to allocate queue family properties for physical device\n");
				return MU_FALSE;
			}
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, queue_family_properties);

			muBool found_graphics = MU_FALSE, found_present = MU_FALSE;

			for (uint32_t i = 0; i < queue_family_property_count; i++) {
				if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					found_graphics = MU_TRUE;
					graphics_family = (uint32_t)i;
				}

				VkBool32 present_support = VK_TRUE;
				vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);
				if (present_support) {
					found_present = MU_TRUE;
					present_family = (uint32_t)i;
				}

				if (found_graphics && found_present) {
					break;
				}
			}

			free(queue_family_properties);

			if (!found_graphics) {
				printf("[WARNING] Failed to find graphics queue family\n");
				return MU_FALSE;
			}

			if (!found_present) {
				printf("[WARNING] Failed to find presentation queue family\n");
				return MU_FALSE;
			}

			/* Queues */
			// The process here is a bit gross, but it does work for 2 families

			float priority = 1.f;

			VkDeviceQueueCreateInfo graphics_device_queue_ci = MU_ZERO_STRUCT(VkDeviceQueueCreateInfo);
			graphics_device_queue_ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphics_device_queue_ci.queueFamilyIndex = graphics_family;
			graphics_device_queue_ci.queueCount = 1;
			graphics_device_queue_ci.pQueuePriorities = &priority;

			VkDeviceQueueCreateInfo present_device_queue_ci = MU_ZERO_STRUCT(VkDeviceQueueCreateInfo);
			present_device_queue_ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			present_device_queue_ci.queueFamilyIndex = present_family;
			present_device_queue_ci.queueCount = 1;
			present_device_queue_ci.pQueuePriorities = &priority;

			VkDeviceQueueCreateInfo device_queue_cis[] = { graphics_device_queue_ci, present_device_queue_ci };
			uint32_t device_queue_ci_count = 2;
			if (graphics_family == present_family) {
				device_queue_ci_count = 1;
			}

			/* Device */

			VkDeviceCreateInfo device_ci = MU_ZERO_STRUCT(VkDeviceCreateInfo);
			device_ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			device_ci.queueCreateInfoCount = device_queue_ci_count;
			device_ci.pQueueCreateInfos = device_queue_cis;
			device_ci.enabledExtensionCount = EXTENSION_COUNT;
			device_ci.ppEnabledExtensionNames = extensions;

			if (use_validation_layers) {
				device_ci.enabledLayerCount = VALIDATION_LAYER_COUNT;
				device_ci.ppEnabledLayerNames = validation_layers;
			}

			VkPhysicalDeviceFeatures features = MU_ZERO_STRUCT(VkPhysicalDeviceFeatures);
			device_ci.pEnabledFeatures = &features;

			if (vkCreateDevice(physical_device, &device_ci, 0, &device) != VK_SUCCESS) {
				printf("[WARNING] Failed to create device\n");
				return MU_FALSE;
			}

			/* Queues */

			vkGetDeviceQueue(device, graphics_family, 0, &graphics_queue);
			vkGetDeviceQueue(device, present_family, 0, &present_queue);

			return MU_TRUE;
		}

		muBool vk_destroy_device(void) {
			if (device != VK_NULL_HANDLE) {
				vkDestroyDevice(device, 0);
			}
			return MU_TRUE;
		}

	/* Swapchain */

		muBool vk_create_swapchain(void) {
			VkSwapchainCreateInfoKHR sc_ci = MU_ZERO_STRUCT(VkSwapchainCreateInfoKHR);
			sc_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			sc_ci.surface = surface;
			sc_ci.imageArrayLayers = 1;
			sc_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			sc_ci.clipped = VK_TRUE;
			sc_ci.oldSwapchain = VK_NULL_HANDLE;

			uint32_t families[2] = { graphics_family, present_family };
			if (graphics_family != present_family) {
				sc_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				sc_ci.queueFamilyIndexCount = 2;
				sc_ci.pQueueFamilyIndices = families;
			} else {
				sc_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			}

			/* Capabilities */

			VkSurfaceCapabilitiesKHR cap;
			if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &cap) != VK_SUCCESS) {
				printf("[WARNING] Failed to get surface capabilities\n");
				return MU_FALSE;
			}

			sc_ci.minImageCount = cap.minImageCount + 1;
			if (cap.maxImageCount > 0 && sc_ci.minImageCount > cap.maxImageCount) {
				sc_ci.minImageCount = cap.maxImageCount;
			}

			sc_ci.preTransform = cap.currentTransform;
			sc_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

			/* Format (based on surface) */

			uint32_t format_count = 0;
			if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, 0) != VK_SUCCESS) {
				printf("[WARNING] Failed to get surface format count\n");
				return MU_FALSE;
			}

			if (format_count == 0) {
				printf("[WARNING] No surface formats found\n");
				return MU_FALSE;
			}

			VkSurfaceFormatKHR* formats = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * format_count);
			if (formats == 0) {
				printf("[WARNING] Failed to allocate storage for surface formats\n");
				return MU_FALSE;
			}
			if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats) != VK_SUCCESS) {
				printf("[WARNING] Failed to get surface formats\n");
				free(formats);
				return MU_FALSE;
			}

			VkSurfaceFormatKHR chosen_format = formats[0];
			for (uint32_t i = 0; i < format_count; i++) {
				if (formats[i].format == VK_FORMAT_R8G8B8A8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					chosen_format = formats[i];
					break;
				}
			}

			free(formats);

			sc_ci.imageFormat = chosen_format.format;
			swapchain_format = sc_ci.imageFormat;
			sc_ci.imageColorSpace = chosen_format.colorSpace;

			/* Extent */

			if (cap.currentExtent.width != UINT32_MAX) {
				sc_ci.imageExtent = cap.currentExtent;
			} else {
				uint32_t width=0, height=0;
				mu_window_get_dimensions(&res, win, &width, &height); scall(mu_window_get_dimensions)

				sc_ci.imageExtent.width = width;
				if (sc_ci.imageExtent.width < cap.minImageExtent.width) {
					sc_ci.imageExtent.width = cap.minImageExtent.width;
				} else if (sc_ci.imageExtent.width > cap.maxImageExtent.width) {
					sc_ci.imageExtent.width = cap.maxImageExtent.width;
				}

				sc_ci.imageExtent.height = height;
				if (sc_ci.imageExtent.height < cap.minImageExtent.height) {
					sc_ci.imageExtent.height = cap.minImageExtent.height;
				} else if (sc_ci.imageExtent.height > cap.maxImageExtent.height) {
					sc_ci.imageExtent.height = cap.maxImageExtent.height;
				}
			}
			swapchain_extent = sc_ci.imageExtent;

			/* Present modes (how images are presented/buffered/preserved) */
			// (see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPresentModeKHR.html)

			uint32_t present_mode_count = 0;
			if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, 0) != VK_SUCCESS) {
				printf("[WARNING] Failed to get surface present mode count\n");
				return MU_FALSE;
			}

			if (present_mode_count == 0) {
				printf("[WARNING] Failed to find any surface present modes\n");
				return MU_FALSE;
			}

			VkPresentModeKHR* present_modes = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * present_mode_count);
			if (present_modes == 0) {
				printf("[WARNING] Failed to allocate memory for storing surface present modes\n");
				return MU_FALSE;
			}
			if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes) != VK_SUCCESS) {
				printf("[WARNING] Failed to get surface present modes\n");
				free(present_modes);
				return MU_FALSE;
			}

			VkPresentModeKHR chosen_present_mode = present_modes[0];
			for (uint32_t i = 0; i < present_mode_count; i++) {
				if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
					chosen_present_mode = present_modes[i];
					break;
				}
			}

			free(present_modes);

			sc_ci.presentMode = chosen_present_mode;

			/* Creation */

			if (vkCreateSwapchainKHR(device, &sc_ci, 0, &swapchain) != VK_SUCCESS) {
				printf("[WARNING] Failed to create swapchain\n");
				return MU_FALSE;
			}

			/* Images */

			if (vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, 0) != VK_SUCCESS) {
				printf("[WARNING] Failed to get swapchain image count\n");
				return MU_FALSE;
			}

			swapchain_images = (VkImage*)malloc(sizeof(VkImage) * swapchain_image_count);
			if (swapchain_images == 0) {
				printf("[WARNING] Failed to allocate swapchain images\n");
				return MU_FALSE;
			}
			if (vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, swapchain_images) != VK_SUCCESS) {
				printf("[WARNING] Failed to get swapchain images\n");
				return MU_FALSE;
			}

			/* Image views */

			swapchain_image_views = (VkImageView*)malloc(sizeof(VkImageView) * swapchain_image_count);
			if (swapchain_image_views == 0) {
				printf("[WARNING] Failed to allocate swapchain image views\n");
				return MU_FALSE;
			}

			for (uint32_t i = 0; i < swapchain_image_count; i++) {
				swapchain_image_views[i] = VK_NULL_HANDLE;
			}

			for (uint32_t i = 0; i < swapchain_image_count; i++) {
				VkImageViewCreateInfo iv_ci = MU_ZERO_STRUCT(VkImageViewCreateInfo);
				iv_ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				iv_ci.image = swapchain_images[i];
				iv_ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
				iv_ci.format = swapchain_format;
				iv_ci.components.r = iv_ci.components.g = iv_ci.components.b = iv_ci.components.a = 
				VK_COMPONENT_SWIZZLE_IDENTITY;
				iv_ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				iv_ci.subresourceRange.baseMipLevel = 0;
				iv_ci.subresourceRange.levelCount = 1;
				iv_ci.subresourceRange.baseArrayLayer = 0;
				iv_ci.subresourceRange.layerCount = 1;

				if (vkCreateImageView(device, &iv_ci, 0, &swapchain_image_views[i]) != VK_SUCCESS) {
					printf("[WARNING] Failed to create one of the swapchain image views\n");
					return MU_FALSE;
				}
			}

			return MU_TRUE;
		}

		muBool vk_destroy_swapchain(void) {
			if (swapchain_image_views != 0) {
				for (uint32_t i = 0; i < swapchain_image_count; i++) {
					if (swapchain_image_views[i] != VK_NULL_HANDLE) {
						vkDestroyImageView(device, swapchain_image_views[i], 0);
					}
				}
				free(swapchain_image_views);
			}
			if (swapchain_images != 0) {
				free(swapchain_images);
			}
			if (swapchain != VK_NULL_HANDLE) {
				vkDestroySwapchainKHR(device, swapchain, 0);
			}
			return MU_TRUE;
		}

	/* Command pool */

		muBool vk_create_command_buffer(void) {
			/* Command pool */

			VkCommandPoolCreateInfo cp_ci = MU_ZERO_STRUCT(VkCommandPoolCreateInfo);
			cp_ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cp_ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			cp_ci.queueFamilyIndex = graphics_family;

			if (vkCreateCommandPool(device, &cp_ci, 0, &command_pool) != VK_SUCCESS) {
				printf("[WARNING] Failed to create command pool\n");
				return MU_FALSE;
			}

			/* Command buffer */

			VkCommandBufferAllocateInfo alloc_info = MU_ZERO_STRUCT(VkCommandBufferAllocateInfo);
			alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			alloc_info.commandPool = command_pool;
			alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			alloc_info.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(device, &alloc_info, &command_buffer) != VK_SUCCESS) {
				printf("[WARNING] Failed to allocate command buffers\n");
				return MU_FALSE;
			}

			/* Semaphores */

			VkSemaphoreCreateInfo s_ci = MU_ZERO_STRUCT(VkSemaphoreCreateInfo);
			s_ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(device, &s_ci, 0, &queue_wait_semaphore) != VK_SUCCESS) {
				printf("[WARNING] Failed to create wait semaphore\n");
				return MU_FALSE;
			}
			if (vkCreateSemaphore(device, &s_ci, 0, &queue_signal_semaphore) != VK_SUCCESS) {
				printf("[WARNING] Failed to create wait semaphore\n");
				return MU_FALSE;
			}

			/* Fence */

			VkFenceCreateInfo f_ci = MU_ZERO_STRUCT(VkFenceCreateInfo);
			f_ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			f_ci.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Create signaled so that the first call to
			// vkWaitForFences passes

			if (vkCreateFence(device, &f_ci, 0, &queue_wait_fence) != VK_SUCCESS) {
				printf("[WARNING] Failed to create fence\n");
				return MU_FALSE;
			}

			return MU_TRUE;
		}

		muBool vk_destroy_command_buffer(void) {
			if (queue_wait_fence != VK_NULL_HANDLE) {
				vkDestroyFence(device, queue_wait_fence, 0);
			}
			if (queue_wait_semaphore != VK_NULL_HANDLE) {
				vkDestroySemaphore(device, queue_wait_semaphore, 0);
			}
			if (queue_signal_semaphore != VK_NULL_HANDLE) {
				vkDestroySemaphore(device, queue_signal_semaphore, 0);
			}
			if (command_buffer != VK_NULL_HANDLE) {
				vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
			}
			if (command_pool != VK_NULL_HANDLE) {
				vkDestroyCommandPool(device, command_pool, 0);
			}
			return MU_TRUE;
		}

	/* Shader modules */

		muBool vk_create_shader_modules(void) {
			VkShaderModuleCreateInfo vertex_ci = MU_ZERO_STRUCT(VkShaderModuleCreateInfo);
			vertex_ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vertex_ci.codeSize = VERTEX_CODE_BYTES;
			vertex_ci.pCode = (const uint32_t*)vertex_code;

			if (vkCreateShaderModule(device, &vertex_ci, 0, &vertex_module) != VK_SUCCESS) {
				printf("[WARNING] Failed to create vertex shader module\n");
				return MU_FALSE;
			}

			VkShaderModuleCreateInfo frag_ci = MU_ZERO_STRUCT(VkShaderModuleCreateInfo);
			frag_ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			frag_ci.codeSize = FRAGMENT_CODE_BYTES;
			frag_ci.pCode = (const uint32_t*)fragment_code;

			if (vkCreateShaderModule(device, &frag_ci, 0, &fragment_module) != VK_SUCCESS) {
				printf("[WARNING] Failed to create fragment shader module\n");
				return MU_FALSE;
			}

			return MU_TRUE;
		}

		muBool vk_destroy_shader_modules(void) {
			if (vertex_module != VK_NULL_HANDLE) {
				vkDestroyShaderModule(device, vertex_module, 0);
			}
			if (fragment_module != VK_NULL_HANDLE) {
				vkDestroyShaderModule(device, fragment_module, 0);
			}

			return MU_TRUE;
		}

	/* Render pass */

		muBool vk_create_render_pass(void) {
			// Color attachment usage; the swapchain usage.
			VkAttachmentDescription attr = MU_ZERO_STRUCT(VkAttachmentDescription);
			attr.format = swapchain_format;
			attr.samples = VK_SAMPLE_COUNT_1_BIT;
			// Before being used (drawing to it), clear contents
			attr.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			// After being used, store its contents
			attr.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			// We don't care what's done to the stencil, since we're not using it.
			attr.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attr.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			// Before being used, we don't care about what the image layout is.
			attr.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			// After being used, we want the image layout to be presentable to the surface.
			attr.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			// Attachment for the subpass, which will describe its respective color attachment,
			// which, in our case, we're just using the color attachment as described by the
			// swapchain. In GLSL, this directly corresponds with the fragment shader output.
			VkAttachmentReference ref = MU_ZERO_STRUCT(VkAttachmentReference);
			ref.attachment = 0;
			ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			// Subpass, which describes how we take in attachment input and output attachment data.
			// This is primarily made for deferred shading pipelines, where multiple passes are
			// sequenced one after the other to achieve post-processing effects, but since we're
			// rendering a simple triangle, a single subpass will do.
			VkSubpassDescription subpass = MU_ZERO_STRUCT(VkSubpassDescription);
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &ref;

			VkRenderPassCreateInfo rp_ci = MU_ZERO_STRUCT(VkRenderPassCreateInfo);
			rp_ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			rp_ci.attachmentCount = 1;
			rp_ci.pAttachments = &attr;
			rp_ci.subpassCount = 1;
			rp_ci.pSubpasses = &subpass;

			if (vkCreateRenderPass(device, &rp_ci, 0, &render_pass) != VK_SUCCESS) {
				printf("[WARNING] Failed to create render pass\n");
				return MU_FALSE;
			}

			return MU_TRUE;
		}

		muBool vk_destroy_render_pass(void) {
			if (render_pass != VK_NULL_HANDLE) {
				vkDestroyRenderPass(device, render_pass, 0);
			}
			return MU_TRUE;
		}

	/* Framebuffers */

		muBool vk_create_framebuffers(void) {
			framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * swapchain_image_count);
			if (framebuffers == 0) {
				printf("[WARNING] Failed to allocate framebuffers\n");
				return MU_FALSE;
			}
			for (uint32_t i = 0; i < swapchain_image_count; i++) {
				framebuffers[i] = VK_NULL_HANDLE;
			}

			for (uint32_t i = 0; i < swapchain_image_count; i++) {
				VkFramebufferCreateInfo ci = MU_ZERO_STRUCT(VkFramebufferCreateInfo);
				ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				ci.renderPass = render_pass;
				ci.attachmentCount = 1;
				ci.pAttachments = &swapchain_image_views[i];
				ci.width = swapchain_extent.width;
				ci.height = swapchain_extent.height;
				ci.layers = 1;

				if (vkCreateFramebuffer(device, &ci, 0, &framebuffers[i]) != VK_SUCCESS) {
					printf("[WARNING] Failed to create a framebuffer\n");
					return MU_FALSE;
				}
			}

			return MU_TRUE;
		}

		muBool vk_destroy_framebuffers(void) {
			if (framebuffers != 0) {
				for (uint32_t i = 0; i < swapchain_image_count; i++) {
					if (framebuffers[i] != VK_NULL_HANDLE) {
						vkDestroyFramebuffer(device, framebuffers[i], 0);
					}
				}
				free(framebuffers);
			}
			return MU_TRUE;
		}

	/* Pipeline */

		muBool vk_create_pipeline(void) {
			VkGraphicsPipelineCreateInfo pipeline_ci = MU_ZERO_STRUCT(VkGraphicsPipelineCreateInfo);
			pipeline_ci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

			// Shader stages

			VkPipelineShaderStageCreateInfo vert_shader_stage_ci = MU_ZERO_STRUCT(VkPipelineShaderStageCreateInfo);
			vert_shader_stage_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vert_shader_stage_ci.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vert_shader_stage_ci.module = vertex_module;
			vert_shader_stage_ci.pName = "main";

			VkPipelineShaderStageCreateInfo frag_shader_stage_ci = MU_ZERO_STRUCT(VkPipelineShaderStageCreateInfo);
			frag_shader_stage_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			frag_shader_stage_ci.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			frag_shader_stage_ci.module = fragment_module;
			frag_shader_stage_ci.pName = "main";

			VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_ci, frag_shader_stage_ci };
			pipeline_ci.stageCount = 2;
			pipeline_ci.pStages = shader_stages;

			// Vertex input

			VkPipelineVertexInputStateCreateInfo vertex_input_ci = MU_ZERO_STRUCT(VkPipelineVertexInputStateCreateInfo);
			vertex_input_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			pipeline_ci.pVertexInputState = &vertex_input_ci;

			// Input assembly

			VkPipelineInputAssemblyStateCreateInfo input_asm_ci = MU_ZERO_STRUCT(VkPipelineInputAssemblyStateCreateInfo);
			input_asm_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			input_asm_ci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			input_asm_ci.primitiveRestartEnable = VK_FALSE;

			pipeline_ci.pInputAssemblyState = &input_asm_ci;

			// Tessellation state

			// ...

			// Viewport state

			swapchain_viewport = MU_ZERO_STRUCT(VkViewport);
			swapchain_viewport.x = 0.f;
			swapchain_viewport.y = 0.f;
			swapchain_viewport.width = (float)swapchain_extent.width;
			swapchain_viewport.height = (float)swapchain_extent.height;
			swapchain_viewport.minDepth = 0.f;
			swapchain_viewport.maxDepth = 1.f;

			swapchain_scissor = MU_ZERO_STRUCT(VkRect2D);
			swapchain_scissor.extent.width = (float)swapchain_extent.width;
			swapchain_scissor.extent.height = (float)swapchain_extent.height;

			VkPipelineViewportStateCreateInfo viewport_state_ci = MU_ZERO_STRUCT(VkPipelineViewportStateCreateInfo);
			viewport_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewport_state_ci.viewportCount = 1;
			viewport_state_ci.pViewports = &swapchain_viewport;
			viewport_state_ci.scissorCount = 1;
			viewport_state_ci.pScissors = &swapchain_scissor;

			pipeline_ci.pViewportState = &viewport_state_ci;

			// Rasterization state

			VkPipelineRasterizationStateCreateInfo raster_state_ci = MU_ZERO_STRUCT(VkPipelineRasterizationStateCreateInfo);
			raster_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			raster_state_ci.depthClampEnable = VK_FALSE;
			raster_state_ci.rasterizerDiscardEnable = VK_FALSE;
			raster_state_ci.polygonMode = VK_POLYGON_MODE_FILL;
			raster_state_ci.cullMode = VK_CULL_MODE_NONE;
			raster_state_ci.depthBiasEnable = VK_FALSE;
			raster_state_ci.lineWidth = 1.f;

			pipeline_ci.pRasterizationState = &raster_state_ci;

			// Multisample state

			VkPipelineMultisampleStateCreateInfo sample_state_ci = MU_ZERO_STRUCT(VkPipelineMultisampleStateCreateInfo);
			sample_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			sample_state_ci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			sample_state_ci.sampleShadingEnable = VK_FALSE;
			sample_state_ci.alphaToCoverageEnable = VK_FALSE;
			sample_state_ci.alphaToOneEnable = VK_FALSE;

			pipeline_ci.pMultisampleState = &sample_state_ci;

			// Depth/Stencil state

			VkPipelineDepthStencilStateCreateInfo ds_state_ci = MU_ZERO_STRUCT(VkPipelineDepthStencilStateCreateInfo);
			ds_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			ds_state_ci.depthTestEnable = VK_FALSE;
			ds_state_ci.depthBoundsTestEnable = VK_FALSE;
			ds_state_ci.stencilTestEnable = VK_FALSE;

			pipeline_ci.pDepthStencilState = &ds_state_ci;

			// Color blend state

			VkPipelineColorBlendAttachmentState blend_attachment = MU_ZERO_STRUCT(VkPipelineColorBlendAttachmentState);
			blend_attachment.blendEnable = VK_FALSE;
			blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | 
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

			VkPipelineColorBlendStateCreateInfo blend_ci = MU_ZERO_STRUCT(VkPipelineColorBlendStateCreateInfo);
			blend_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			blend_ci.logicOpEnable = VK_FALSE;
			blend_ci.attachmentCount = 1;
			blend_ci.pAttachments = &blend_attachment;

			pipeline_ci.pColorBlendState = &blend_ci;

			// Dynamic state

			VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

			VkPipelineDynamicStateCreateInfo dynamic_ci = MU_ZERO_STRUCT(VkPipelineDynamicStateCreateInfo);
			dynamic_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamic_ci.dynamicStateCount = sizeof(dynamic_states) / sizeof(VkDynamicState);
			dynamic_ci.pDynamicStates = dynamic_states;

			pipeline_ci.pDynamicState = &dynamic_ci;

			// Pipeline layout

			VkPipelineLayoutCreateInfo pipeline_layout_ci = MU_ZERO_STRUCT(VkPipelineLayoutCreateInfo);
			pipeline_layout_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

			if (vkCreatePipelineLayout(device, &pipeline_layout_ci, 0, &pipeline_layout) != VK_SUCCESS) {
				printf("[WARNING] Failed to create pipeline layout\n");
				return MU_FALSE;
			}

			pipeline_ci.layout = pipeline_layout;

			// Other stuff

			pipeline_ci.renderPass = render_pass;
			pipeline_ci.subpass = 0;

			if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_ci, 0, &pipeline) != VK_SUCCESS) {
				printf("[WARNING] Failed to create graphics pipeline\n");
				return MU_FALSE;
			}

			return MU_TRUE;
		}

		muBool vk_destroy_pipeline(void) {
			if (pipeline != VK_NULL_HANDLE) {
				vkDestroyPipeline(device, pipeline, 0);
			}
			if (pipeline_layout != VK_NULL_HANDLE) {
				vkDestroyPipelineLayout(device, pipeline_layout, 0);
			}
			return MU_TRUE;
		}

	/* Termination */

		muBool vk_term(void) {
			vkDeviceWaitIdle(device);

			vk_destroy_pipeline();
			vk_destroy_framebuffers();
			vk_destroy_render_pass();
			vk_destroy_shader_modules();
			vk_destroy_command_buffer();
			vk_destroy_swapchain();

			vk_destroy_device();
			vk_destroy_physical_device();
			vk_destroy_surface();
			vk_destroy_debug_messenger();
			vk_destroy_instance();

			return MU_TRUE;
		}

/* Shader code */

	// Vertex code compiled from:
	/* shader.vert
	#version 450
	
	vec2 vVertices[3] = vec2[](
		vec2(0.0, -0.5),
		vec2(0.5, 0.5),
		vec2(-0.5, 0.5)
	);

	layout (location=0) out vec3 fColor;

	vec3 fColors[3] = vec3[](
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0)
	);

	void main(void) {
		gl_Position = vec4(vVertices[gl_VertexIndex], 0.0, 1.0);
		fColor = fColors[gl_VertexIndex];
	}
	*/

	// Fragment code compiled from:
	/* shader.frag
	#version 450

	layout (location=0) in vec3 fColor;
	layout (location=0) out vec4 foColor;

	void main(void) {
		foColor = vec4(fColor, 1.0);
	}
	*/

	muByte vertex_code[VERTEX_CODE_BYTES] = {
		0x03,0x02,0x23,0x07,0x00,0x00,0x01,0x00,0x0b,0x00,0x0d,0x00,0x36,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00,0x11,0x00,0x02,0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x06,0x00
		,0x01,0x00,0x00,0x00,0x47,0x4c,0x53,0x4c,0x2e,0x73,0x74,0x64,0x2e,0x34,0x35,0x30
		,0x00,0x00,0x00,0x00,0x0e,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00
		,0x0f,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e
		,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x26,0x00,0x00,0x00,0x31,0x00,0x00,0x00
		,0x03,0x00,0x03,0x00,0x02,0x00,0x00,0x00,0xc2,0x01,0x00,0x00,0x04,0x00,0x0a,0x00
		,0x47,0x4c,0x5f,0x47,0x4f,0x4f,0x47,0x4c,0x45,0x5f,0x63,0x70,0x70,0x5f,0x73,0x74
		,0x79,0x6c,0x65,0x5f,0x6c,0x69,0x6e,0x65,0x5f,0x64,0x69,0x72,0x65,0x63,0x74,0x69
		,0x76,0x65,0x00,0x00,0x04,0x00,0x08,0x00,0x47,0x4c,0x5f,0x47,0x4f,0x4f,0x47,0x4c
		,0x45,0x5f,0x69,0x6e,0x63,0x6c,0x75,0x64,0x65,0x5f,0x64,0x69,0x72,0x65,0x63,0x74
		,0x69,0x76,0x65,0x00,0x05,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e
		,0x00,0x00,0x00,0x00,0x05,0x00,0x05,0x00,0x0c,0x00,0x00,0x00,0x76,0x56,0x65,0x72
		,0x74,0x69,0x63,0x65,0x73,0x00,0x00,0x00,0x05,0x00,0x04,0x00,0x17,0x00,0x00,0x00
		,0x66,0x43,0x6f,0x6c,0x6f,0x72,0x73,0x00,0x05,0x00,0x06,0x00,0x20,0x00,0x00,0x00
		,0x67,0x6c,0x5f,0x50,0x65,0x72,0x56,0x65,0x72,0x74,0x65,0x78,0x00,0x00,0x00,0x00
		,0x06,0x00,0x06,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x67,0x6c,0x5f,0x50
		,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x00,0x06,0x00,0x07,0x00,0x20,0x00,0x00,0x00
		,0x01,0x00,0x00,0x00,0x67,0x6c,0x5f,0x50,0x6f,0x69,0x6e,0x74,0x53,0x69,0x7a,0x65
		,0x00,0x00,0x00,0x00,0x06,0x00,0x07,0x00,0x20,0x00,0x00,0x00,0x02,0x00,0x00,0x00
		,0x67,0x6c,0x5f,0x43,0x6c,0x69,0x70,0x44,0x69,0x73,0x74,0x61,0x6e,0x63,0x65,0x00
		,0x06,0x00,0x07,0x00,0x20,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x67,0x6c,0x5f,0x43
		,0x75,0x6c,0x6c,0x44,0x69,0x73,0x74,0x61,0x6e,0x63,0x65,0x00,0x05,0x00,0x03,0x00
		,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x06,0x00,0x26,0x00,0x00,0x00
		,0x67,0x6c,0x5f,0x56,0x65,0x72,0x74,0x65,0x78,0x49,0x6e,0x64,0x65,0x78,0x00,0x00
		,0x05,0x00,0x04,0x00,0x31,0x00,0x00,0x00,0x66,0x43,0x6f,0x6c,0x6f,0x72,0x00,0x00
		,0x48,0x00,0x05,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00,0x48,0x00,0x05,0x00,0x20,0x00,0x00,0x00,0x01,0x00,0x00,0x00
		,0x0b,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x48,0x00,0x05,0x00,0x20,0x00,0x00,0x00
		,0x02,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x48,0x00,0x05,0x00
		,0x20,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x04,0x00,0x00,0x00
		,0x47,0x00,0x03,0x00,0x20,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x47,0x00,0x04,0x00
		,0x26,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x2a,0x00,0x00,0x00,0x47,0x00,0x04,0x00
		,0x31,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x00,0x02,0x00
		,0x02,0x00,0x00,0x00,0x21,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00
		,0x16,0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x17,0x00,0x04,0x00
		,0x07,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x15,0x00,0x04,0x00
		,0x08,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x00,0x04,0x00
		,0x08,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x1c,0x00,0x04,0x00
		,0x0a,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x20,0x00,0x04,0x00
		,0x0b,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x3b,0x00,0x04,0x00
		,0x0b,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x2b,0x00,0x04,0x00
		,0x06,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x00,0x04,0x00
		,0x06,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0xbf,0x2c,0x00,0x05,0x00
		,0x07,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x0e,0x00,0x00,0x00
		,0x2b,0x00,0x04,0x00,0x06,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x3f
		,0x2c,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x10,0x00,0x00,0x00
		,0x10,0x00,0x00,0x00,0x2c,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x12,0x00,0x00,0x00
		,0x0e,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x2c,0x00,0x06,0x00,0x0a,0x00,0x00,0x00
		,0x13,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x12,0x00,0x00,0x00
		,0x17,0x00,0x04,0x00,0x14,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03,0x00,0x00,0x00
		,0x1c,0x00,0x04,0x00,0x15,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x09,0x00,0x00,0x00
		,0x20,0x00,0x04,0x00,0x16,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x15,0x00,0x00,0x00
		,0x3b,0x00,0x04,0x00,0x16,0x00,0x00,0x00,0x17,0x00,0x00,0x00,0x06,0x00,0x00,0x00
		,0x2b,0x00,0x04,0x00,0x06,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x80,0x3f
		,0x2c,0x00,0x06,0x00,0x14,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x18,0x00,0x00,0x00
		,0x0d,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x2c,0x00,0x06,0x00,0x14,0x00,0x00,0x00
		,0x1a,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x0d,0x00,0x00,0x00
		,0x2c,0x00,0x06,0x00,0x14,0x00,0x00,0x00,0x1b,0x00,0x00,0x00,0x0d,0x00,0x00,0x00
		,0x0d,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x2c,0x00,0x06,0x00,0x15,0x00,0x00,0x00
		,0x1c,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x1b,0x00,0x00,0x00
		,0x17,0x00,0x04,0x00,0x1d,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x04,0x00,0x00,0x00
		,0x2b,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x01,0x00,0x00,0x00
		,0x1c,0x00,0x04,0x00,0x1f,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x1e,0x00,0x00,0x00
		,0x1e,0x00,0x06,0x00,0x20,0x00,0x00,0x00,0x1d,0x00,0x00,0x00,0x06,0x00,0x00,0x00
		,0x1f,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x21,0x00,0x00,0x00
		,0x03,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x21,0x00,0x00,0x00
		,0x22,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x15,0x00,0x04,0x00,0x23,0x00,0x00,0x00
		,0x20,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,0x23,0x00,0x00,0x00
		,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x25,0x00,0x00,0x00
		,0x01,0x00,0x00,0x00,0x23,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x25,0x00,0x00,0x00
		,0x26,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x28,0x00,0x00,0x00
		,0x06,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x2e,0x00,0x00,0x00
		,0x03,0x00,0x00,0x00,0x1d,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x30,0x00,0x00,0x00
		,0x03,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x30,0x00,0x00,0x00
		,0x31,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x33,0x00,0x00,0x00
		,0x06,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x36,0x00,0x05,0x00,0x02,0x00,0x00,0x00
		,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0xf8,0x00,0x02,0x00
		,0x05,0x00,0x00,0x00,0x3e,0x00,0x03,0x00,0x0c,0x00,0x00,0x00,0x13,0x00,0x00,0x00
		,0x3e,0x00,0x03,0x00,0x17,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x3d,0x00,0x04,0x00
		,0x23,0x00,0x00,0x00,0x27,0x00,0x00,0x00,0x26,0x00,0x00,0x00,0x41,0x00,0x05,0x00
		,0x28,0x00,0x00,0x00,0x29,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x27,0x00,0x00,0x00
		,0x3d,0x00,0x04,0x00,0x07,0x00,0x00,0x00,0x2a,0x00,0x00,0x00,0x29,0x00,0x00,0x00
		,0x51,0x00,0x05,0x00,0x06,0x00,0x00,0x00,0x2b,0x00,0x00,0x00,0x2a,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00,0x51,0x00,0x05,0x00,0x06,0x00,0x00,0x00,0x2c,0x00,0x00,0x00
		,0x2a,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x50,0x00,0x07,0x00,0x1d,0x00,0x00,0x00
		,0x2d,0x00,0x00,0x00,0x2b,0x00,0x00,0x00,0x2c,0x00,0x00,0x00,0x0d,0x00,0x00,0x00
		,0x18,0x00,0x00,0x00,0x41,0x00,0x05,0x00,0x2e,0x00,0x00,0x00,0x2f,0x00,0x00,0x00
		,0x22,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x3e,0x00,0x03,0x00,0x2f,0x00,0x00,0x00
		,0x2d,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x23,0x00,0x00,0x00,0x32,0x00,0x00,0x00
		,0x26,0x00,0x00,0x00,0x41,0x00,0x05,0x00,0x33,0x00,0x00,0x00,0x34,0x00,0x00,0x00
		,0x17,0x00,0x00,0x00,0x32,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x14,0x00,0x00,0x00
		,0x35,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x3e,0x00,0x03,0x00,0x31,0x00,0x00,0x00
		,0x35,0x00,0x00,0x00,0xfd,0x00,0x01,0x00,0x38,0x00,0x01,0x00
	};
	muByte fragment_code[FRAGMENT_CODE_BYTES] = {
		0x03,0x02,0x23,0x07,0x00,0x00,0x01,0x00,0x0b,0x00,0x0d,0x00,0x13,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x11,0x00,0x02,0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x06,0x00,
		0x01,0x00,0x00,0x00,0x47,0x4c,0x53,0x4c,0x2e,0x73,0x74,0x64,0x2e,0x34,0x35,0x30,
		0x00,0x00,0x00,0x00,0x0e,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
		0x0f,0x00,0x07,0x00,0x04,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e,
		0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x10,0x00,0x03,0x00,
		0x04,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x02,0x00,0x00,0x00,
		0xc2,0x01,0x00,0x00,0x04,0x00,0x0a,0x00,0x47,0x4c,0x5f,0x47,0x4f,0x4f,0x47,0x4c,
		0x45,0x5f,0x63,0x70,0x70,0x5f,0x73,0x74,0x79,0x6c,0x65,0x5f,0x6c,0x69,0x6e,0x65,
		0x5f,0x64,0x69,0x72,0x65,0x63,0x74,0x69,0x76,0x65,0x00,0x00,0x04,0x00,0x08,0x00,
		0x47,0x4c,0x5f,0x47,0x4f,0x4f,0x47,0x4c,0x45,0x5f,0x69,0x6e,0x63,0x6c,0x75,0x64,
		0x65,0x5f,0x64,0x69,0x72,0x65,0x63,0x74,0x69,0x76,0x65,0x00,0x05,0x00,0x04,0x00,
		0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e,0x00,0x00,0x00,0x00,0x05,0x00,0x04,0x00,
		0x09,0x00,0x00,0x00,0x66,0x6f,0x43,0x6f,0x6c,0x6f,0x72,0x00,0x05,0x00,0x04,0x00,
		0x0c,0x00,0x00,0x00,0x66,0x43,0x6f,0x6c,0x6f,0x72,0x00,0x00,0x47,0x00,0x04,0x00,
		0x09,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x00,0x04,0x00,
		0x0c,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x00,0x02,0x00,
		0x02,0x00,0x00,0x00,0x21,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
		0x16,0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x17,0x00,0x04,0x00,
		0x07,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x20,0x00,0x04,0x00,
		0x08,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,
		0x08,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x17,0x00,0x04,0x00,
		0x0a,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x20,0x00,0x04,0x00,
		0x0b,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,
		0x0b,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,
		0x06,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x80,0x3f,0x36,0x00,0x05,0x00,
		0x02,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
		0xf8,0x00,0x02,0x00,0x05,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x0a,0x00,0x00,0x00,
		0x0d,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x51,0x00,0x05,0x00,0x06,0x00,0x00,0x00,
		0x0f,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x00,0x05,0x00,
		0x06,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
		0x51,0x00,0x05,0x00,0x06,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,
		0x02,0x00,0x00,0x00,0x50,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x12,0x00,0x00,0x00,
		0x0f,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
		0x3e,0x00,0x03,0x00,0x09,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0xfd,0x00,0x01,0x00,
		0x38,0x00,0x01,0x00
	};

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2024 Hum
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

