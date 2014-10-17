enhwin.exe: enhwin.obj enhframe.obj
        tlink -Toe -aa -x c02.obj enhwin.obj enhframe.obj, enhwin.exe,,\
                                 c2 os2
enhwin.obj: enhwin.c
        bcc -c enhwin.c

enhframe.obj: enhframe.c
        bcc -c enhframe.c
