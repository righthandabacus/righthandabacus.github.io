---
layout: post
title: "Graph paper by MetaPost"
date: 2007-03-27 15:31:26 +0800
tags: code
---

With MetaPost, we can generate a graph paper (i.e. the square grids) easily.
Below is a script on MetaPost that generates a graph paper on an A4 sheet, using
1mm by 1mm minor grid and 10mm by 10mm major grid, with 10mm margin on four
sides. All lines are in grey color for the convenience of using it with black
pens.

```latex
% MetaPost script
% to generate a graph paper in A4
beginfig(1)

margin=10mm;        % Margin of the graph paper
minor=1mm;          % Minor lines, each every 1mm
major=10mm;         % Major lines, each every 10mm
                    % major must be a multiple of minor

z1 = (margin,margin);   % lower left corner
z2 = (210mm,297mm)-z1;  % upper right corner

% Use the thick pen first
pickup pencircle scaled 0.5;

% Draw the frame
draw z1--(x1,y2)--z2--(x2,y1)--cycle withcolor .5white;

% Use the very thin pen
pickup pencircle scaled 0.1;

% Draw the vertical lines
for i=minor step minor until (y2-y1-minor):
    if (i mod major)=0:
        pickup pencircle scaled 0.3;
    fi;
    draw (z1 shifted (0,i))--((x2,y1) shifted (0,i)) withcolor .4white;
    if (i mod major)=0:
        pickup pencircle scaled 0.1;
    fi;
endfor;

% Draw the horizonal lines
for i=minor step minor until (x2-x1-minor):
    if (i mod major)=0:
        pickup pencircle scaled 0.3;
    fi;
    draw ( z1--(x1,y2) ) shifted (i,0) withcolor .4white;
    if (i mod major)=0:
        pickup pencircle scaled 0.1;
    fi;
endfor;

endfig;
end;
```

The output postscript file is here: [`graphpaper.ps`](/img/graphpaper.ps), and
you can also download the above code directly here:
[`graphpaper.mp`](/img/graphpaper.mp).
