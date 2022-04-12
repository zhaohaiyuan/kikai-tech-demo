# KiKaiTech Pedestrian Detection
## Development Environment
- Window 10 x64
- CMake version 2.8.8 (or higher)
- Visual Studio 2015
## Dependencies (3rd party)
- OpenCV version 3.4.0
- [OpenVINO version 2021.2.185](https://docs.openvino.ai/2021.2/index.html)
- [json](https://github.com/nlohmann/json)
## Pedestrian Detection Model
This project use the pre-trained of Intel [pedestrian-detection-adas-0002](https://docs.openvino.ai/latest/omz_models_model_pedestrian_detection_adas_0002.html)
## Build with Visual Studio 2015
1. Checkout the project in git
2. Change directory to 'build' folder
3. Run cmake command: 
    ```bash
    cmake -G "Visual Studio 14 Win64" ..
    ```
4. Open the DemoProj.sln
## Run the application
1. Create the application config in json format, named is app_config.json, the config file have the same directory as the application.
2. Start the application
