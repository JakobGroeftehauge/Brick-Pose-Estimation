print('Testing generator.py ... ')

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))
import keras_retinanet.preprocessing  # noqa: F401
__package__ = "keras_retinanet.preprocessing"


from .csv_generator import CSVGenerator
from ..utils.transform import random_transform_generator

transform_generator = random_transform_generator(flip_x_chance=1.0)

# Create train CSV generator
class_path = 'keras_retinanet/preprocessing/Test-files/classes.csv'
annotation_path = 'keras_retinanet/preprocessing/Test-files/annotations.csv'
generator = CSVGenerator(annotation_path, class_path, transform_generator=transform_generator)


group = generator.groups[0]

# load images and annotations
print("Load annotations: ")
image_group       = generator.load_image_group(group)
annotations_group = generator.load_annotations_group(group)

#print(annotations_group)
print("bboxes: ", annotations_group[0]["bboxes"])
print("labels: ", annotations_group[0]["labels"])
print("angles: ", annotations_group[0]["angles"])

# check validity of annotations
print("validation of annotations - No output is prefered")
image_group, annotations_group = generator.filter_annotations(image_group, annotations_group, group)

# randomly transform data
print("transform data")
image_group, annotations_group = generator.random_transform_group(image_group, annotations_group)

#print(annotations_group)
print("bboxes: ", annotations_group[0]["bboxes"])
print("labels: ", annotations_group[0]["labels"])
print("angles: ", annotations_group[0]["angles"])

targets = generator.compute_targets(image_group, annotations_group)

angles_batch, regression_batch, labels_batch = targets
print("Angle Batch", angles_batch)
print("Regression Batch", regression_batch)
print("Classification batch", labels_batch)
print(angles_batch.shape)
print(regression_batch.shape)
print(labels_batch.shape)
print(sum(labels_batch[0, :, 0]))
