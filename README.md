# openSTBT (Open Set Top Box Testing)

This project aims to automate Set Top Box testing. Initial commit does the following:
- Takes the reference image and the streamed image as inputs
- Mask unwanted regions, if required
- Calculate PSNR
- Pass/Fail criteria based on PSNR threshold

Platform: x86/x64 running any Linux based OS. Run make to create the binary.
