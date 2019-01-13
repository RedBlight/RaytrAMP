clc;
clear all;

c0 = 299792458.0;
freq = 1.645E9;
lambda = 100 * c0 / freq;
wh = 30.2;
wr = 6.48;
br = 6.48;
tr = 25;
ch = 46;
pinl = lambda / 4.56;
fw = - wh + lambda / 4;

n = -10 : 10;
upval = ((-70/360)+n)*lambda;
sideval = (1/tan(30*pi/180)) * upval;

distval = (0+70/360)*lambda / sin(30*pi/180);
