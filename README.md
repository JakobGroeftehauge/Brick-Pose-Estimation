# Deep Learning Aided Pose Estimation for Robotic Brick Picking

## Abstract

This project aims to examine different approaches to computer vision based object detec-tion and orientation estimation, in the context of automating brick work. More specifically thedepalletising task. For detecting bricks, four different solutions are proposed in the project,two methods based on traditional computer vision and two based on the deep learning detec-tor RetinaNet. The traditional methods, which are considered as baselines, are based aroundedge maps found using the Canny edge detection algorithm. The first method uses the edgemap for line extraction utilising the Hough transform. The other is a variation of the cham-fer matching algorithm. RetinaNet does not offer orientation estimation, thus the two deeplearning based methods aim to adapt RetinaNet to accommodate this. The first deep learningmethod adds an extra sub-network, for regressing the orientation angle, to RetinaNet. For thesecond method, the model RotinaNet is proposed, which features changes to allow predictionof tight fitting oriented bounding boxes.  For evaluation and model training purposes, twodifferent data sets are produced, and each split into a training and a validation partition. Thedeep learning methods are trained on the training sets, and all methods are evaluated on thevalidation sets. The proposed RotinaNet is found to yield the best performance out of theproposed methods, even showing a significant increase in detection performance over vanillaRetinaNet, for the given application.

## Contents

### Baseline Methods 
* Detection of Lines using Hough Lines
* Chamfer Matching

### Deep Learning Based Methods 
* Vanila RetinaNet - Axis Aligned Bounding Boxes (https://github.com/fizyr/keras-retinanet)
* RetinaNet with Angle - Axis Alinged with Added Seperate Angle Subnet
* RotinaNet - Non-Axis Aligned with Combined Bounding Box regression and Angle subnet. 

### Data 
* Simple Dataset - Single Layer of Bricks
* On Pallet Dataset - Multiple Stacked Layers of Bricks

## Future Work
* CenterNet - Objects as Points (https://arxiv.org/abs/1904.07850)

