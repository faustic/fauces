# w65c02sxbrun

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Description

This is a command line utility that sends a test program to a W65C02SXB card, waits for the card to complete execution and then retrieves the result.

W65C02SXB is a card with a 65C02 processor, which we wanted to use to compare our emulator's behaviour with that of a real processor.

We cannot guarantee that it works on any W65C02SXB card. It has been tested on a W65C02SXB card labelled as *Rev. C*, whose installed ROM has the following signature:

    8000  57 44 43 ff 4c 15 81 ff  57 44 43 36 35 63 30 32  |WDC.L...WDC65c02|
    8010  53 4b 20 57 44 43 4d 4f  4e 20 56 65 72 73 69 6f  |SK WDCMON Versio|
    8020  6e 20 3d 20 20 32 2e 30  2e 34 2e 33 56 65 72 73  |n =  2.0.4.3Vers|
    8030  69 6f 6e 20 44 61 74 65  20 3d 20 54 75 65 20 4a  |ion Date = Tue J|
    8040  75 6c 20 20 32 20 32 30  31 33 20 31 36 3a 32 34  |ul  2 2013 16:24|


We learned how to communicate with the card thanks to [Andrew Jacob's Uploader source code](https://github.com/andrew-jacobs/dev65/blob/master/src/com/wdc65xx/sxb/Uploader.java).

Syntax:

    w65c02sxbrun --input <executable> --output <result> --port <serialport>

