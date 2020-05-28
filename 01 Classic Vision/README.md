# Baseline Methods - Traditional Vision

# Methods
## Line Based Detection 

### Include Files
**Brick_Detector.h:** Class which defines functionality for the line extraction based brick detection method.<br/>
**Hough_space.h:** Class which implements the required functionality for computing the Hough transform.<br/>


## Chamfer Matching

### Include Files
**Chamfer_brick_detector.h:** Class which defines functionality for the chamfer matching based brick detection method.<br/>

## Miscellaneous Include Files 
**json.hpp:** Library for reading and writting json-files (Copied from: https://github.com/nlohmann/json) <br/>
**annotation_file.h:** Defines the layout of an annotation. <br/>
**predictions.h:** Contains definitions of different structs used in the project.<br/>
**data_loader.h:** Supplies an image and corresponding annotation file. <br/>
**annotation_loader.h:** Loads 'LabelMe' annotations stored in .json files. <br/>
**depth_processor.h:** Setup for processing the depth information from the camera. Used for collecting data.<br/>
**realsense_camera.h:** Setup for the camera used for collecting data. <br/>
**util.h:** Contains utility functions. Mostly related to drawing figures. <br/>
**Evaluator.h:** Class which handles functionality related to evalution of the developed methods. <br/>
**Detector.h:** Parent class for detection methods. The line extraction based and chamfer matching based methods inherits from this class.<br/>

# Projects 
**BB_characteristics:**  Generates raw information regardng the annotated bounding boxes in a data set <br/>
**Baseline_method:** Initial application for performing grid search to determine hyperparameters for the bline based algorithm <br/>
**Chamfer_evaluation:** Evaluation of the chamfer matching algorithm using axis aligned bounding box as target <br/>
**Eval_Hough:** Evalution of the line based method, using both rotated and axis aligned bounding boxes represenation <br/>
**Eval_chamfer_rotated:** Evaluation of the chamfer matching algorithm using rotated rectangle as target <br/> 
**Vision_assisted_annotation.** Application for generating initial annotations to images, using the chamfer matching methods <br/>
**determine_brick_specs:** Determines size specification of all the annotations in a data set <br/> 
**real_sense:** Application for Capturing images via the intel RealSense camera <br/>
