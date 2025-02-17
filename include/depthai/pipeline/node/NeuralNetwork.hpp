#pragma once

#include "depthai/openvino/OpenVINO.hpp"
#include "depthai/pipeline/Node.hpp"

// standard
#include <fstream>

// shared
#include <depthai-shared/properties/NeuralNetworkProperties.hpp>

namespace dai {
namespace node {

/**
 * @brief NeuralNetwork node. Runs a neural inference on input data.
 */
class NeuralNetwork : public Node {
   public:
    using Properties = dai::NeuralNetworkProperties;

    std::string getName() const override;

   protected:
    nlohmann::json getProperties() override;
    std::shared_ptr<Node> clone() override;
    tl::optional<OpenVINO::Version> getRequiredOpenVINOVersion() override;

   private:
    Properties properties;

   protected:
    OpenVINO::Version networkOpenvinoVersion;
    virtual Properties& getPropertiesRef();

   public:
    NeuralNetwork(const std::shared_ptr<PipelineImpl>& par, int64_t nodeId);

    /**
     * Input message with data to be inferred upon
     * Default queue is blocking with size 5
     */
    Input input{*this, "in", Input::Type::SReceiver, true, 5, {{DatatypeEnum::Buffer, true}}};

    /**
     * Outputs NNData message that carries inference results
     */
    Output out{*this, "out", Output::Type::MSender, {{DatatypeEnum::NNData, false}}};

    /**
     * Passthrough message on which the inference was performed.
     *
     * Suitable for when input queue is set to non-blocking behavior.
     */
    Output passthrough{*this, "passthrough", Output::Type::MSender, {{DatatypeEnum::Buffer, true}}};

    // Specify local filesystem path to load the blob (which gets loaded at loadAssets)
    /**
     * Load network blob into assets and use once pipeline is started.
     *
     * Throws if file doesn't exist or isn't a valid network blob.
     * @param path Path to network blob
     */
    void setBlobPath(const std::string& path);

    /**
     * Specifies how many frames will be available in the pool
     * @param numFrames How many frames will pool have
     */
    void setNumPoolFrames(int numFrames);

    /**
     * How many threads should the node use to run the network.
     * @param numThreads Number of threads to dedicate to this node
     */
    void setNumInferenceThreads(int numThreads);

    /**
     * How many Neural Compute Engines should a single thread use for inference
     * @param numNCEPerThread Number of NCE per thread
     */
    void setNumNCEPerInferenceThread(int numNCEPerThread);

    /**
     * How many inference threads will be used to run the network
     * @returns Number of threads, 0, 1 or 2. Zero means AUTO
     */
    int getNumInferenceThreads();
    // TODO add getters for other API
};

}  // namespace node
}  // namespace dai
