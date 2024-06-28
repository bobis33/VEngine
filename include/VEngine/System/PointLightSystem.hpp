/*
** @file PointLightSystem.hpp
** @brief This file contains the PointLightSystem class
** @namespace ven
*/

#pragma once

#include <memory>
#include <vector>

#include "VEngine/Camera.hpp"
#include "VEngine/Constant.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"
#include "VEngine/Object.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    class PointLightSystem {

    public:

        explicit PointLightSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();

        PointLightSystem(const PointLightSystem&) = delete;
        PointLightSystem& operator=(const PointLightSystem&) = delete;

        void render(FrameInfo &frameInfo);

    private:

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        Device &m_device;

        std::unique_ptr<Shaders> m_shaders;
        VkPipelineLayout m_pipelineLayout{nullptr};

    }; // class PointLightSystem

} // namespace ven
