unplot2d
========

This is the my first experimental work to read numerical values
from the image file of the scientific / techinical plot 2d.
To simplify problems, it treats clean and naive images.

 1. just created by a plot application.
 1. noiseless, clean, and rotateless.
 1. 300 dpi, black-and-white image.
 1. a plot area is surrounded by a rectangle.
 1. the plot area drawn by only one kind of data mark.
 1. all of the data marks are separeted from each others.
 1. data marks may put on the frame or axis.
 1. scale ticks drawn on both X- and Y-axis.
 1. scale digits drawn for both X- and Y-axis.
 1. text labels drawn for both X- and Y-axis.

Build
-----

opencv 2.4 or later and tesseract-ocr 3.03.02 or later are required to build.
If you build on Ubuntu linux box:

    $ sudo apt-get install libopencv-dev libtesseract-dev
    $ make unplot2d
    $ ./unplot2d example.png
    "d/nm","Tc/K"
    0.991561,2.49928
    1.00211,2.56151
    1.49789,2.46165
    1.99367,2.12446
    2.5,1.99855
    3.99789,1.97395
    5,2.01158
    10,1.99855
    $ make clean

License
------

The BSD 3-Clause

Copyright (c) 2017, MIZUTANI Tociyuki
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
