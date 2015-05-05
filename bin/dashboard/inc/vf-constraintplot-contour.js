(function () {


d3.geom.contourOriginal = function(grid, griddata) {
 
 
	var s =  d3_geom_contourStartOriginal(grid, griddata); // starting point
	 if (s == null) 
       return c;
	 
	 var c = [],    // contour polygon
      x = s[0],  // current x position
      y = s[1],  // current y position
      dx = 0,    // next x direction
      dy = 0,    // next y direction
      pdx = NaN, // previous x direction
      pdy = NaN, // previous y direction
      i = 0;
	 
  do {
    //console.debug("starting i: " + i + " x,y(" + x + "," + y + ") d(" + dx + "," + dy + ") pd(" + pdx + "," + pdy + ")");
    // determine marching squares index
    i = 0;
    if (grid(x-1, y-1)) i += 1;
    if (grid(x,   y-1)) i += 2;
    if (grid(x-1, y  )) i += 4;
    if (grid(x,   y  )) i += 8;

    // determine next direction
    if (i === 6) {
      dx = pdy === -1 ? -1 : 1;
      dy = 0;
    } else if (i === 9) {
      dx = 0;
      dy = pdx === 1 ? -1 : 1;
    } else {
      dx = d3_geom_contourDx[i];
      dy = d3_geom_contourDy[i];
    }
    // update contour polygon
    if (dx != pdx && dy != pdy) {
      c.push([x, y]);
      pdx = dx;
      pdy = dy;
    }
    x += dx;
    y += dy;
  } while (s[0] != x || s[1] != y);
  return c;
};

/*
This is similar to d3.geom.contourOriginal
but check if path go thorugh outer bound then return undefine so not to draw any path.
This is for gridValue = 0;
This difference between contourOriginal and contourOriginalZero is just below code and passing arguments.

*/
d3.geom.contourOriginalZero = function(grid, griddata, xMaxIndex, yMaxIndex) {
	var s =  d3_geom_contourStartOriginal(grid, griddata); // starting point
	 if (s == null) 
       return c;
	 
	 var c = [],    // contour polygon
      x = s[0],  // current x position
      y = s[1],  // current y position
      dx = 0,    // next x direction
      dy = 0,    // next y direction
      pdx = NaN, // previous x direction
      pdy = NaN, // previous y direction
      i = 0;
	 
  do {
    //console.debug("starting i: " + i + " x,y(" + x + "," + y + ") d(" + dx + "," + dy + ") pd(" + pdx + "," + pdy + ")");
    // determine marching squares index
    i = 0;
    if (grid(x-1, y-1)) i += 1;
    if (grid(x,   y-1)) i += 2;
    if (grid(x-1, y  )) i += 4;
    if (grid(x,   y  )) i += 8;

    // determine next direction
    if (i === 6) {
      dx = pdy === -1 ? -1 : 1;
      dy = 0;
    } else if (i === 9) {
      dx = 0;
      dy = pdx === 1 ? -1 : 1;
    } else {
      dx = d3_geom_contourDx[i];
      dy = d3_geom_contourDy[i];
    }
    // update contour polygon
    if (dx != pdx && dy != pdy) {
	  if ( x == 0 || x == xMaxIndex || y == 0 || y == yMaxIndex)
		return undefined;
      c.push([x, y]);
      pdx = dx;
      pdy = dy;
    }
	
    x += dx;
    y += dy;
  } while (s[0] != x || s[1] != y);
  return c;
};

function d3_geom_contourStartOriginal(grid, griddata) {
  var x = 0,
      y = 0;

  // search for a starting point; begin at origin
  // and proceed along outward-expanding diagonals
  while (true) {
    if (grid(x,y)) {
      return [x,y];
    }
    if (x === 0) {
	  
      x = y + 1;
      y = 0;
	  //console.debug("searching next in1: " + x + ", " + y );
    } else {
      x = x - 1;
      y = y + 1;

	if ( x === griddata.length -1 && y === griddata.length -1 )
		return null;
	  //console.debug("searching next in2: " + x + ", " + y );
    }
  }
}

    // lookup tables for marching directions
    var d3_geom_contourDx = [1, 0, 1, 1, -1, 0, -1, 1, 0, 0, 0, 0, -1, 0, -1, NaN],
        d3_geom_contourDy = [0, -1, 0, 0, 0, -1, 0, 0, 1, -1, 1, 1, 0, -1, 0, NaN];
})();
