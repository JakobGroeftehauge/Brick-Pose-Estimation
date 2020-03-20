print('Testing generator.py ... ')

import os
import sys
import cv2

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))
import keras_retinanet.preprocessing  # noqa: F401
__package__ = "keras_retinanet.preprocessing"


from .csv_generator import CSVGenerator
from ..utils.transform import random_transform_generator
from ..utils.visualization import draw_box, draw_vector


#transform_generator = random_transform_generator(flip_x_chance=0)
transform_generator = random_transform_generator(
            min_rotation=-0.5,
            max_rotation=0.5,
            min_translation=(-0.1, -0.1),
            max_translation=(0.1, 0.1),
            min_shear= 0.0, # -0.1,
            max_shear= 0.0, #0.1,
            min_scaling=(0.9, 0.9),
            max_scaling=(1.1, 1.1),
            flip_x_chance=0,
            flip_y_chance=0,
        )

# Create train CSV generator
class_path = 'keras_retinanet/preprocessing/Test-files/classes.csv'
annotation_path = 'keras_retinanet/preprocessing/Test-files/annotations_bp.csv'
generator = CSVGenerator(annotation_path, class_path, transform_generator=transform_generator)

#print(generator.image_data)


group = generator.groups[0]

# load images and annotations
print("Load annotations: ")
image_group       = generator.load_image_group(group)
annotations_group = generator.load_annotations_group(group)

#print(annotations_group)
#print("bboxes: ", annotations_group[0]["bboxes"])
#print("labels: ", annotations_group[0]["labels"])
#print("angles: ", annotations_group[0]["angles"])

# check validity of annotations
#print("validation of annotations - No output is prefered")
image_group, annotations_group = generator.filter_annotations(image_group, annotations_group, group)

# randomly transform data
#print("transform data")
image_group, annotations_group = generator.random_transform_group(image_group, annotations_group)


#print(annotations_group)
#print("bboxes: ", annotations_group[0]["bboxes"])
#print("labels: ", annotations_group[0]["labels"])
print("angles: ", annotations_group[0]["angles"])
print("bboxes",  annotations_group[0]["bboxes"])
print("classification: ",  annotations_group[0]["labels"])

draw = image_group[0].copy()

for box, angle in zip(annotations_group[0]["bboxes"],annotations_group[0]["angles"]):
    # scores are sorted so we can break
    b = box.astype(int)
    draw_box(draw, b, color=(0,0,255))
    draw_vector(draw, angle, box)

cv2.imshow("test", draw)
cv2.waitKey(0)
'''
targets = generator.compute_targets(image_group, annotations_group)

regression_batch, labels_batch, angles_batch = targets
print("Angle Batch", angles_batch)
print("Regression Batch", regression_batch)
print("Classification batch", labels_batch)
print(angles_batch.shape)
print(regression_batch.shape)
print(labels_batch.shape)
print(sum(labels_batch[0, :, 0]))
'''

#print(angles_batch[:20])
