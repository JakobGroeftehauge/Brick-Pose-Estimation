# Baseline Methods - Traditional Vision

# Methods
## Line Based Detection 

### Include Files
**Brick_Detector.h:** <br/>
**Hough_space.h:** <br/>


## Chamfer Matching

### Include Files
**Chamfer_brick_detector.h:**  <br/>

## Miscellaneous Include Files 
**json.hpp:** Library for reading and writting json-files (Copied from: https://github.com/nlohmann/json) <br/>
**annotation_file.hpp:** <br/>
**predictions.h:** <br/>
**data_loader.h:**  <br/>
**annotation_loader.h:**  <br/>
**depth_processor.h:**  <br/>
**realsense_camera.h:**  <br/>
**util.h:** <br/>
**Evaluator.h:**  <br/>
**Detector.h:** <br/>

# Projects 
**BB_characteristics:**  Generates raw information regardng the annotated bounding boxes in a data set <br/>
**Baseline_method:** Initial application for performing grid search to determine hyperparameters for the line based algorithm <br/>
**Chamfer_evaluation:** Evaluation of the chamfer matching algorithm using axis aligned bounding box as target <br/>
**Eval_Hough:** Evalution of the line based method, using both rotated and axis aligned bounding boxes represenation <br/>
**Eval_chamfer_rotated:** Evaluation of the chamfer matching algorithm using rotated rectangle as target <br/> 
**Vision_assisted_annotation.** Application for generating initial annotations to images, using the chamfer matching methods<br/>
**determine_brick_specs:** Determines size specification of all the annotations in a data set <br/> 
**real_sense:** Application for Capturing images via the intel RealSense camera <br/>
