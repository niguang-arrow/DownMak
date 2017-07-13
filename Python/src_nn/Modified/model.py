import torch
import torch.nn as nn
from numpy.random import normal
from numpy.linalg import svd
from math import sqrt


def _get_orthogonal_init_weights(weights):
    fan_out = weights.size(0)
    fan_in = weights.size(1) * weights.size(2) * weights.size(3)

    u, _, v = svd(normal(0.0, 1.0, (fan_out, fan_in)), full_matrices=False)

    if u.shape == (fan_out, fan_in):
        return torch.Tensor(u.reshape(weights.size()))
    else:
        return torch.Tensor(v.reshape(weights.size()))


class SRCNN(nn.Module):

    """Docstring for SRCNN. """

    def __init__(self):
        super(SRCNN, self).__init__()
        self.fea_extract = nn.Conv2d(1, 64, 9, stride=1, padding=4)
        self.nonlinear = nn.Conv2d(64, 32, 1, stride=1, padding=0)
        self.reconstruct = nn.Conv2d(32, 1, 5, stride=1, padding=2)
        self.relu = nn.ReLU()

        self._initialize_weights()

    def forward(self, input):
        x = self.relu(self.fea_extract(input))
        x = self.relu(self.nonlinear(x))
        x = self.reconstruct(x)
        return x

    def _initialize_weights(self):
        self.fea_extract.weight.data.copy_(_get_orthogonal_init_weights(self.fea_extract.weight) * sqrt(2))
        self.nonlinear.weight.data.copy_(_get_orthogonal_init_weights(self.nonlinear.weight) * sqrt(2))
        self.reconstruct.weight.data.copy_(_get_orthogonal_init_weights(self.reconstruct.weight) * sqrt(2))


if __name__ == '__main__':
    from torch.autograd import Variable

    model = SRCNN()
    input = Variable(torch.randn(1, 1, 33, 33))
    print model
    output = model(input)
    print output.size()
