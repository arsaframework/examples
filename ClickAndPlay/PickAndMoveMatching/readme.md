PickAndMoveMatching
Result:

![result](PickAndMoveMatching.gif)


Source code:
```
{
	screensize(640,960)
	set($prefix_obj, "obj")
	set($prefix_point, "point")
	set($prefix_shadow, "shadow")

	function(reset)
}function(init)

{
	set( $ox, getorix($name) )
	set( $oy, getoriy($name) )
	pos($name, $ox, $oy)
	visible(true,$name)
}function(reset_layer_pos, $name)

{
	set($state,STATE_IDLE)	
	set($score, 0)
	visible(false,"win")
	getlayerall($prefix_shadow, setcolor("null", 0,0,0,255)) // set shadow all black
	getlayerall($prefix_obj, function(reset_layer_pos,getlayername("null")) )
}function(reset)

{
	set($state,STATE_DOWN)
	builtin($layername,"r=0x0x10x20x0x1 rf=c")
	setalpha($layername, 128)
}function(selected)

{
	set($state,STATE_IDLE)
	builtin($layername,"r=0x0x0x0x0x1")
	setalpha($layername, 255)
}function(released)

{
	function(released) 
	pos($layername,$shadowname)
	setcolor($shadowname,255,255,255,255)
	add($score,1)
	visible(false,$layername)
	set($layername,"")	
}function(hit)

// idle to layername
if($state==STATE_IDLE) if(touch("press", $prefix_obj)==true) set($layername, getlayername("null"))

// check layername pickup
if($layername == "") return()

// prepare prefix
set( $pointname, string.replace($layername, $prefix_obj, $prefix_point) )
set( $shadowname, string.replace($layername, $prefix_obj, $prefix_shadow) )

// idle and layername to move
if($state==STATE_IDLE) if(touch("isleft", $layername)==true) function(selected)
if($state==STATE_IDLE) if(touch("isright", $layername)==true) function(selected)
if($state==STATE_IDLE) if(touch("isup", $layername)==true) function(selected)
if($state==STATE_IDLE) if(touch("isdown", $layername)==true) function(selected)

// return to idle
if($state==STATE_DOWN) if(touch("release")==true) function(released)

// hit
hitlayer($pointname, $layername, function(hit) )

// show win
if($score >= 3 ) visible(true,"win")

// move machine
if($state==STATE_DOWN) if(touch("isleft")==true) move($layername, -500, 0)
if($state==STATE_DOWN) if(touch("isright")==true) move($layername, 500, 0)
if($state==STATE_DOWN) if(touch("isup")==true) move($layername, 0, -500)
if($state==STATE_DOWN) if(touch("isdown")==true) move($layername, 0, 500)

// clamp pos
set($x, getposx($layername))
set($y, getposy($layername))
set($w, getwidth($layername))
set($h, getheight($layername))
set($sw, getscreensizewidth())
set($sh, getscreensizeheight())
if($x < 0 ) posx($layername, 0)
if($y < 120 ) posy($layername, 120)
if($x > $sw-$w ) posx($layername, $sw-$w)
if($y > $sh-$h ) posy($layername, $sh-$h)

if(touch("release", "reset")==true) function(reset)

text(25,10,"Score: $score", 0,0,0,255)
```