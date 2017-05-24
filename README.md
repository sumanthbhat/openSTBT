# openSTBT (Open Set Top Box Testing)

This project aims to automate Set Top Box testing. Initial commit does the following:
- Takes the reference image and the streamed image as inputs
- Mask unwanted regions, if required
- Calculate PSNR
- Pass/Fail criteria based on PSNR threshold

Usage   : PSNR image-1.yuv image-2.yuv [options]

Options : -m Enable masking
Platform: x86/x64 running any Linux based OS. Run make to create the binary.
