PickAndMove
Result:

![result](PickAndMove.gif)


Source code:
```
if(touch("isleft", "bird")==true) move("null", -500, 0)
if(touch("isright", "bird")==true) move("null", 500, 0)
if(touch("isup", "bird")==true) move("null", 0, -500)
if(touch("isdown", "bird")==true) move("null", 0, 500)
```