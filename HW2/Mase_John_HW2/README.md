# Name: John (JJ) Mase
# USC ID: 6904908117
# Email: mase@usc.edu
# Visual Studio Version: 16.11.0
# Other Information
## DDA Structs
I've defined two additional structs for the scan-line algorithm. These structs (DDA and DDA_Span) are defined in rend.h, as informed to do by Yiqi.
The DDA struct is used for interpolating the Y dimension and the DDA_Span struct is used for interpolating the X dimension. Since computing the X
values requires less information than computing the Y values, I've opted to use two different but similar structs instead of ignoring the extra
variables present in one.