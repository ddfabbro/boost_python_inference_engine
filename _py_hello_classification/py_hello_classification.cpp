#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "inference_engine.hpp"
#include "samples/ocv_common.hpp"
#include "samples/classification_results.h"

#include "boost/python.hpp"

namespace py = boost::python;

cv::Mat imreadW(std::wstring input_image_path)
{
    cv::Mat image;
    std::ifstream input_image_stream;

    input_image_stream.open(input_image_path.c_str(), std::iostream::binary | std::ios_base::ate | std::ios_base::in);

    if (input_image_stream.is_open())
    {
        if (input_image_stream.good())
        {
            std::size_t file_size = input_image_stream.tellg();
            input_image_stream.seekg(0, std::ios::beg);
            std::vector<char> buffer(0);

            std::copy(std::istream_iterator<char>(input_image_stream), std::istream_iterator<char>(), std::back_inserter(buffer));
            image = cv::imdecode(cv::Mat(1, file_size, CV_8UC1, &buffer[0]), cv::IMREAD_COLOR);
        }

        else
        {
            std::wcout << "Input file '" << input_image_path << "' processing error" << std::endl;
        }

        input_image_stream.close();
    }

    else
    {
        std::wcout << "Unable to read input file '" << input_image_path << "'" << std::endl;
    }

    return image;
}

void hello_classification(const std::wstring input_model, const std::wstring input_image_path, const std::string device_name)
{
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 1. Load inference engine instance -------------------------------------
    InferenceEngine::Core ie;
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 2. Read IR Generated by ModelOptimizer (.xml and .bin files) ------------
    InferenceEngine::CNNNetwork network = ie.ReadNetwork(input_model, input_model.substr(0, input_model.size() - 4) + L".bin");
    network.setBatchSize(1);
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 3. Configure input & output ---------------------------------------------
    // --------------------------- Prepare input blobs -----------------------------------------------------
    InferenceEngine::InputInfo::Ptr input_info = network.getInputsInfo().begin()->second;
    std::string input_name = network.getInputsInfo().begin()->first;

    /* Mark input as resizable by setting of a resize algorithm.
    * In this case we will be able to set an input blob of any shape to an infer request.
    * Resize and layout conversions are executed automatically during inference */
    input_info->getPreProcess().setResizeAlgorithm(InferenceEngine::RESIZE_BILINEAR);
    input_info->setLayout(InferenceEngine::Layout::NHWC);
    input_info->setPrecision(InferenceEngine::Precision::U8);

    // --------------------------- Prepare output blobs ----------------------------------------------------
    InferenceEngine::DataPtr output_info = network.getOutputsInfo().begin()->second;
    std::string output_name = network.getOutputsInfo().begin()->first;

    output_info->setPrecision(InferenceEngine::Precision::FP32);
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 4. Loading model to the device ------------------------------------------
    InferenceEngine::ExecutableNetwork executable_network = ie.LoadNetwork(network, device_name);
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 5. Create infer request -------------------------------------------------
    InferenceEngine::InferRequest infer_request = executable_network.CreateInferRequest();
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 6. Prepare input --------------------------------------------------------
    /* Read input image to a blob and set it to an infer request without resize and layout conversions. */
    cv::Mat image = imreadW(input_image_path);
    InferenceEngine::Blob::Ptr imgBlob = wrapMat2Blob(image);  // just wrap Mat data by Blob::Ptr without allocating of new memory
    infer_request.SetBlob(input_name, imgBlob);  // infer_request accepts input blob of any size
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 7. Do inference --------------------------------------------------------
    /* Running the request synchronously */
    infer_request.Infer();
    // -----------------------------------------------------------------------------------------------------

    // --------------------------- 8. Process output ------------------------------------------------------
    InferenceEngine::Blob::Ptr output = infer_request.GetBlob(output_name);
    // Print classification results
    ClassificationResultW classificationResult(output, { input_image_path });
    classificationResult.print();
    // -----------------------------------------------------------------------------------------------------
}

BOOST_PYTHON_MODULE(_py_hello_classification)
{
    py::def("hello_classification", hello_classification);
}
