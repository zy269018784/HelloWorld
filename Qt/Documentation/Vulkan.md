QVulkanInstance 
QVulkanWindow
QVulkanWindowRenderer
QVulkanLayer
QVulkanExtension
QVulkanQVulkanFunctions
QVulkanInfoVector

QVulkanWindow: Vulkan rendering的QWindow.
    clipCorrectionMatrix: OpenGL和Vulkan坐标系转换矩阵.
    createRenderer: 创建QVulkanWindowRenderer.
    currentFrame: curremt frame index 范围 [0, concurrentFrameCount() - 1].
    currentFramebuffer： 
    depthStencilFormat:  depth stencil buffer的格式.
    depthStencilImage:   depth stencil Image.
    depthStencilImageView:   depth stencil image view.
    frameGrabbed:
    frameReady: image ready信号.
    grab:
    graphicsCommandPool:
    graphicsQueue:
    graphicsQueueFamilyIndex:
    hostVisibleMemoryIndex:
    isValid:
    msaaColorImage:
    physicalDevice: active physical device.
    sampleCountFlagBits:

QVulkanInstance:
    vkInstance: 返回VkInstance handle.

QVulkanWindowRenderer: