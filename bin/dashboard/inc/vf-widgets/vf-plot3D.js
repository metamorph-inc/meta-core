

  function vf_plotIn3D( xmin,xmax, ymin, ymax, zmin, zmax, ztarget, sfunction, scolor, xlabel, ylabel, inequality)
  {

     DomReady.ready(function() {

        ThreeBox.preload([
         // 'build/MathBox.glsl.html', //'shaders/snippets.glsl.html',
          'snippets.glsl.html',
        ], function () {

         var mathbox = window.mathbox = mathBox({
            //OrthographicCamera( left, right, top, bottom, near, far )
           //camera: new THREE.OrthographicCamera( -2, 2,  -2, 2,  1, 100  ),
           //commented out = PerspectiveCamera  
           cameraControls: true,
            cursor:         true,
            controlClass:   ThreeBox.OrbitControls,
            elementResize:  true, //false for OrthographicCamera
            fullscreen:     true,
            screenshot:     true,
            stats:          false,
            scale:          1,
          }).start();

           var ztop = zmax;
           if (ztarget > zmax){
               ztop = ztarget;
           }
           var zbottom = zmin;
           if (ztarget < zmin){
                zbottom = ztarget;
           }
           mathbox
            // Cartesian viewport
            .viewport({
              type: 'cartesian',
              range: [ [xmax, xmin], [zbottom, ztop], [ymin, ymax]],
              scale: [1, 1, 1],
              polar: 0,
            })
            .camera({
                orbit:6,
                phi: -τ/6,
                theta: 0.3,
            })
            .transition(500);

             // Axes
            mathbox.axis({
              id: xlabel,
              axis: 0,
              color: 0xa0a0a0,
              ticks: 5,
              lineWidth: 2,
              size: .05,
              labels: true,
              decimals: 5,
              offset: [0, zbottom, ymin],
            })
            .axis({
              id: 'z-axis',
              axis: 1,
              color: 0xa0a0a0,
              ticks: 5,
              lineWidth: 2,
              size: .05,
              zero: false,
              labels: true,
              decimals: 6,
               offset: [xmin, 0, ymin],
            })
            .axis({
              id: ylabel,
              axis: 2,
              color: 0xa0a0a0,
              ticks: 5,
              lineWidth: 2,
              size: .05,
              zero: false,
              labels: true,
              decimals: 5,
               offset:[xmin,  zbottom, 0],
            })


            // Grid
            .grid({
              axis: [0, 2],
              color: 0xa0a0a0,
               offset: [0,  zbottom, 0],
            })
		    //Constant function
		    .surface({
              shaded: false,
              domain: [[xmax, xmin], [ymin, ymax]],
              n: [2, 2],
              expression: function(){return ztarget;},
              color: 0x000000, //original 0x20C050
              opacity: 0.5, //original 0.5

            })
            // Surface function
            .surface({
                shaded: false,
                domain: [[xmax, xmin], [ymin, ymax]],
                n: [32, 32],
                color: parseInt(scolor,16), //parseInt("0x20C050",16)
                expression: sfunction,
                //expression: function(x,y){ return [x, 2087.849326163504 + 6589.650484379083  * x + 80.71766376839791  * y + -484464.4118859415  * x * x + -7.1769302189811714  * y * y + 290.31427855961044  * x * y, y];},
                opacity: .999, // Bugfix: for some reason, tangent attribute is gone when material.transparent = false
            })
           .curve( {
               id: 'z-target',
               n: 1,
               data: [[xmin, ztarget, ymin]],
               points: true,
               line: false,
               pointSize: 17,
               zIndex: 2,
               color: 0x20B050,
           });


            if ( inequality == "L" && zmax > ztarget && zmin < ztarget )
            {
                mathbox.vector({
                    id: 'bad',
                    n: 2,
                    lineWidth: 10,
                    color: 0xb70000, //red
                    data: [[xmin, ztarget, ymin], [xmin, zmin, ymin] ],
                });

                //in target
                mathbox.vector({
                    id: 'good',
                    n: 2,
                    lineWidth: 10,
                    color: 0x0066aa,//blue
                    data: [ [xmin, ztarget, ymin], [xmin, zmax, ymin]],
                });

            }
            else if (inequality == "S" && zmax > ztarget && zmin < ztarget)
            {
                mathbox.vector({
                    id: 'bad',
                    n: 2,
                    lineWidth: 10,
                    color: 0xb70000, //red
                    data: [[xmin, ztarget, ymin], [xmin, zmax, ymin] ],
                });
                mathbox.vector({
                    id: 'good',
                    n: 2,
                    lineWidth: 10,
                    color: 0x0066aa,//blue
                    data: [ [xmin, ztarget, ymin], [xmin, zmin, ymin]],
                });
            }
            else if  ( inequality == "L" && zmax > ztarget && zmin > ztarget) //only good
            {
                mathbox.vector({
                    id: 'good',
                    n: 2,
                    lineWidth: 10,
                    color: 0x0066aa,//blue
                    data: [ [xmin, ztarget, ymin], [xmin, zmax, ymin]],
                });
            }
            else if  ( inequality == "S" && zmax < ztarget && zmin < ztarget) //only good
            {
                mathbox.vector({
                    id: 'good',
                    n: 2,
                    lineWidth: 10,
                    color: 0x0066aa,//blue
                    data: [ [xmin, ztarget, ymin], [xmin, zmin, ymin]],
                });
            }
            else if  ( inequality == "L" &&  zmax < ztarget && zmin < ztarget) //only bad
            {
               mathbox.vector({
                    id: 'bad',
                    n: 2,
                    lineWidth: 10,
                    color: 0xb70000, //red
                    data: [[xmin, ztarget, ymin], [xmin, zmin, ymin] ],
                });
            }
            else if  ( inequality == "S" &&  zmax > ztarget && zmin > ztarget) //only bad
            {
                mathbox.vector({
                    id: 'bad',
                    n: 2,
                    lineWidth: 10,
                    color: 0xb70000, //red
                    data: [[xmin, ztarget, ymin], [xmin, zmax, ymin] ],
                });
            }
        });

     });

 }



