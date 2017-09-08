#include <iostream>
#include <iterator>
#include <cmath>

using namespace std;


bool convolve1D(float* in, float* out, int dataSize, float* kernel, int kernelSize)
{
    int i, j, k;

    // check validity of params
    if(!in || !out || !kernel) return false;
    if(dataSize <=0 || kernelSize <= 0) return false;

    // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
    for(i = kernelSize-1; i < dataSize; ++i)
    {
        out[i] = 0;                             // init to 0 before accumulate

        for(j = i, k = 0; k < kernelSize; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    // convolution from out[0] to out[kernelSize-2]
    for(i = 0; i < kernelSize - 1; ++i)
    {
        out[i] = 0;                             // init to 0 before sum

        for(j = i, k = 0; j >= 0; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    return true;
}

void conv1d(float *in, float *out, int dataSize, float *kernel, int kernelSize) {

    int kCenter = kernelSize / 2;
    int mm;
    int rowIndex;

    for (int i = 0; i < dataSize; ++i) {
        int sum = 0;
        for (int m = 0; m < kernelSize; ++m) {
            mm = kernelSize - m - 1;
            rowIndex = i + m - kCenter;

            if(rowIndex >= 0 && rowIndex < dataSize)
                sum += in[rowIndex] * kernel[mm];
        }
        out[i] = sum;
    }
}


void convo1d(float* in, float* out, int dataSize, float* kernel, int kernelSize)
{
    int i, m;
    float *inPtr, *inPtr2, *outPtr, *kPtr;
    int kCenter;
    int rowMin, rowMax;                             // to check boundary of input array

    // find center position of kernel (half of kernel size)
    kCenter = kernelSize >> 1;

    // init working  pointers
    inPtr = inPtr2 = &in[kCenter];  // note that  it is shifted (kCenterX, kCenterY),
    outPtr = out;
    kPtr = kernel;

    // start convolution
    for(i = 0; i < dataSize; ++i) {
        // compute the range of convolution, the current column of kernel should be between these
        rowMax = i + kCenter;
        rowMin = i - dataSize + kCenter;

        *outPtr = 0;                            // set to 0 before accumulate

        // flip the kernel and traverse all the kernel values
        // multiply each kernel value with underlying input data
        for(m = 0; m < kernelSize; ++m) {
            // check the boundary of array
            if(m <= rowMax && m > rowMin)
                *outPtr += *(inPtr - m) * *kPtr;
            ++kPtr;                     // next kernel
        }

        kPtr = kernel;                          // reset kernel to (0,0)
        inPtr = ++inPtr2;                       // next input
        ++outPtr;                               // next output
    }
}


int main() {

    float kernel[] = {1, 2, 3};
    float data[] = {1, 2, 3, 4, 5};
    float out[5];
    //convolve1D(data, out, 7, kernel, 3);
    convo1d(data, out, 5, kernel, 3);

    for (auto p = begin(out); p != end(out); ++p)
        cout << *p << " ";
    cout << endl;

    return 0;
}
