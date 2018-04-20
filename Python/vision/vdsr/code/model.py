import torch
import torch.nn as nn
from math import sqrt


class Conv_ReLU_Block(nn.Module):
    def __init__(self):
        super(Conv_ReLU_Block, self).__init__()
        self.conv = nn.Conv2d(64, 64, 3, 1, 1, bias=False)
        self.relu = nn.ReLU(inplace=False)

    def forward(self, input):
        x = self.conv(input)
        x = self.relu(x)
        return x


class VDSR(nn.Module):
    def __init__(self):
        super(VDSR, self).__init__()
        self.residual = self.make_layer(Conv_ReLU_Block, 18)
        self.input = nn.Conv2d(1, 64, 3, 1, 1, bias=False)
        self.output = nn.Conv2d(64, 1, 3, 1, 1, bias=False)
        self.relu = nn.ReLU(inplace=True)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
                m.weight.data.normal_(0, sqrt(2. / n))

    def make_layer(self, block, num_of_layer):
        layers = []
        for _ in range(num_of_layer):
            layers.append(block())

        return nn.Sequential(*layers)

    def forward(self, input):
        x = self.relu(self.input(input))
        x = self.residual(x)
        residual = self.output(x)
        x = torch.add(input, residual)
        return x
