import sys
import os

if __name__ == "__main__" and __package__ is None:
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))
    import keras_retinanet.models  # noqa: F401
    __package__ = "keras_retinanet.models"

from . import retinanet
from . import resnet


model = retinanet.default_submodels_and_angle(1, 300)
print(model)

retinanet_model = resnet.resnet_retinanet(1)
print(retinanet_model.summary())
