/*
$("div.colorSelector").each(function(i){
	 console.log("=====================MJD======================================");
	 console.log(this);
	
	var self = this;
	
	$(this).ColorPicker({
		color: $(self).attr('color'),
		onShow: function(colpkr){
			$(colpkr).fadeIn(500);
			return false;
		},
		onHide: function(colpkr){
			$(colpkr).fadeOut(500);
			return false;
		},
		onChange: function(hbs, hex, rgb){
			vm.listOfPETResponses()[+$(self).attr("index")].color("#" + hex);
		}
	});
});
*/
var gid = {};
function unloadVisualizationArtifacts()
{
	console.debug("unloadVisualizationArtifacts");
	$("#artifactsTarget div").each( function (index, value){
		console.debug( "Stop animating divId = " + this.id );
		cancelAnimationFrame( gid[this.id]);
	});
	$("#artifactsTarget").empty();
	
}

function loadVisualizationArtifacts(files)
{
	console.debug("loadVisualizationArtifacts");
	console.debug($("#artifactsTarget div").length);
	if ($("#artifactsTarget div").length == 0 ){
		for (var i = 0; i < files.length; i++) {
			var filepathsplits = files[i].split('/');
			var title = filepathsplits[filepathsplits.length - 1];
			loadAVisualizationArtifactFile("#artifactsTarget", title, "GLDiv" +i, files[i]);
		}
	}
	else
		console.debug("#artifactsTarget is NOT empty. So ignored.");
	//loadSTLFile("#artifactsTarget", "Solid1", "GLDiv", "/darpa-mw/demo/stl/stlfiles/solid.stl");
	//loadSTLFile("#artifactsTarget", "Peacock_USB-V2", "GLDiv3", "/darpa-mw/demo/stl/stlfiles/Peacock_USB-V2.stl");
	//loadSTLFile("#artifactsTarget", "XmasSet_Coal_X1", "GLDiv2", "/darpa-mw/demo/stl/stlfiles/XmasSet_Coal_X1.stl");
	//loadSTLFile("#artifactsTarget", "nx", "GLDiv5", "/darpa-mw/demo/stl/stlfiles/nx.stl");
}

	
function loadAVisualizationArtifactFile( targetDiv, filename, divId, filepath)
{
	var splits = filename.split('.');
	$("<h4>"+ filepath +"</h4>").appendTo(targetDiv); 
	//$("<h4>"+ filename +"</h4>").appendTo(targetDiv); 
	$("<div class='gldiv' id='" + divId + "'/>").appendTo(targetDiv); 
	//$("<div class='colorSelector' data-bind='attr: {index: $index, color: color}' style='display: inline-block;'><div data-bind='style: {backgroundColor: color}'></div></div>").appendTo(targetDiv); 
	
	if ( filepath.indexOf("http://") != -1){
		loadHTTP(divId, filepath);
	}
	else {
		var ext = splits[splits.length - 1] ;
		if (ext == 'svg') {
			loadSVGViewer(divId, filepath);
		}
		else if (ext == 'png'||ext == 'gif' ||ext == 'jpg'|| ext == 'jpeg' || ext == 'bmp')
			loadImageViewer(divId, filepath, filename);
		else if (ext == 'stl') {
			loadSTLViewer(divId, filepath);
		}
		else if ( ext == 'mp4'){
			loadMP4Video(divId, filepath);
		}
		else if ( ext == 'mov' || ext == 'm4v'){
			loadOtherVideo(divId, filepath);
		}
	}
	
}

function loadSTLViewer(divId, filepath)
{
	var _camera, _scene, _renderer, _trackball, _projector;

	var _entities = [];
	//initGL(divId, 1200);
	initGL(divId, $(window).width());
	
	//var base_width = $(element).parent().width() - padding;
	
	readSTLFile(filepath);//var filepath = "/darpa-mw/demo/stl/stlfiles/solid.stl";
	animate();
	
	function readSTLFile( fpath) {
		var url = fpath; //"readdata.php";
		var result = "";
		$.ajax({
			url: url,
			type: 'GET',
			dataType: 'text',
			success: function (data) {
				result = data;
			   // console.debug(result);
				var meshEntityList = stl2json(result);
				createScene(meshEntityList);
			},
			async: false
		});
	}

	function ConvertClr(clr) {
		var bytes = [];

		bytes[0] = (clr >>> 24) & 0xFF; //R
		bytes[1] = (clr >>> 16) & 0xFF; //G
		bytes[2] = (clr >>> 8) & 0xFF;  //B
		bytes[3] = (clr >>> 0) & 0xFF;  //A

		return bytes[2] | (bytes[1] << 8) | (bytes[0] << 16);
	}

	function clearScene()
	{
		for (var i = 0; i < _entities.length; i++) {
			_scene.remove(_entities[i]);
		}

		_entities = [];
	}

	function createScene(meshDataList){
		clearScene();

		_camera.fov = 40;
		_camera.position.x = 0;
		_camera.position.y = 0;
		_camera.position.z = 30;

		var center = [0.0, 0.0, 0.0];

		var len = meshDataList.length;

		for (var meshIdx = 0; meshIdx < len; meshIdx++) {

			var meshData = meshDataList[meshIdx];

			var geometry = new THREE.Geometry();

			var vertexArray = [];

			//uncompress vertices array
			for (var i = 0; i < meshData.VertexIndices.length; i += 1) {

				var idx = 3 * meshData.VertexIndices[i];

				vertexArray[i] = new THREE.Vector3(
					meshData.VertexCoords[idx],
					meshData.VertexCoords[idx + 1],
					meshData.VertexCoords[idx + 2]);
			}

			var normalArray = [];

			//uncompress normals array
			for (var i = 0; i < meshData.NormalIndices.length; i += 1) {

				var idx = 3 * meshData.NormalIndices[i];

				normalArray[i] = new THREE.Vector3(
					meshData.Normals[idx],
					meshData.Normals[idx + 1],
					meshData.Normals[idx + 2]);
			}

			//Generate Faces
			for (var i = 0; i < vertexArray.length; i += 3) {

				geometry.vertices.push(vertexArray[i]);
				geometry.vertices.push(vertexArray[i + 1]);
				geometry.vertices.push(vertexArray[i + 2]);

				var face = new THREE.Face3(i, i + 1, i + 2)

				geometry.faces.push(face);

				face.vertexNormals.push(normalArray[i]);
				face.vertexNormals.push(normalArray[i + 1]);
				face.vertexNormals.push(normalArray[i + 2]);
			}

			center[0] += meshData.Center[0];
			center[1] += meshData.Center[1];
			center[2] += meshData.Center[2];

			var material = new THREE.MeshLambertMaterial(
			{
				//color: ConvertClr(meshData.Color[0]),
				color: meshData.Color, //expecting color in Hexadecimal (ie., 0xFF0000)
				shading: THREE.SmoothShading
			});
			var body = new THREE.Mesh(geometry, material);

			body.doubleSided = false;

			body.geometry.dynamic = true;
			body.geometry.__dirtyVertices = true;
			body.geometry.__dirtyNormals = true;

			var entity = new THREE.Object3D();

			entity.add(body);

			_entities.push(entity);

			_scene.add(entity);
		}

		center[0] = center[0] / len;
		center[1] = center[1] / len;
		center[2] = center[2] / len;

		for (var i = 0; i < _entities.length; i++) {
			_entities[i].applyMatrix(new THREE.Matrix4().makeTranslation(
			   -center[0],
			   -center[1],
			   -center[2]));
		}
	};

	function hasWebGL() {
		try {
			var canvas = document.createElement('canvas');
			var ret =   !!(window.WebGLRenderingContext &&       (canvas.getContext('webgl') || canvas.getContext('experimental-webgl'))
		);
			return ret;
		}
		catch (e) {
			return false;
		};
	}

	function initGL(gldivId, gldivIdWitdh) {

		var animateWithWebGL = hasWebGL();

		var container = document.getElementById(gldivId);

		_scene = new THREE.Scene();

		var width = gldivIdWitdh - 35;
		var height = 700;

		_camera = new THREE.PerspectiveCamera(40, width / height, 0.1, 500);

		_camera.position.x = 0;
		_camera.position.y = 0;
		_camera.position.z = 50;

		_scene.add(_camera);

		_trackball = new THREE.TrackballControls(_camera, container);
		_trackball.rotateSpeed = 3.5;
		_trackball.zoomSpeed = 2.0;
		_trackball.panSpeed = 0.5;
		_trackball.noZoom = false;
		_trackball.noPan = false;
		_trackball.staticMoving = true;
		_trackball.dynamicDampingFactor = 0.3;
		_trackball.minDistance = 1;
		_trackball.maxDistance = 100;
		_trackball.keys = [82, 90, 80]; // [r:rotate, z:zoom, p:pan]
		//_trackball.addEventListener('change', render);

		// create lights
		var light1 = new THREE.PointLight(0xFFFFFF);
		var light2 = new THREE.PointLight(0xFFFFFF);
		var light3 = new THREE.PointLight(0xFFFFFF);
		var light4 = new THREE.PointLight(0xFFFFFF);

		light1.position.x = 100;
		light1.position.y = 50;
		light1.position.z = 200;

		light2.position.x = -100;
		light2.position.y = 150;
		light2.position.z = -200;

		light3.position.x = 100;
		light3.position.y = -150;
		light3.position.z = -100;

		light4.position.x = -100;
		light4.position.y = -150;
		light4.position.z = 100;

		_scene.add(light1);
		_scene.add(light2);
		_scene.add(light3);
		_scene.add(light4);

		_renderer = new THREE.WebGLRenderer();  //CanvasRenderer();
		_renderer.setSize(width, height);

		_projector = new THREE.Projector();

		container.appendChild(_renderer.domElement);

		//to make mousewheel on all images
		//document.addEventListener('mousewheel', onDocumentMouseWheel, false);
		//to make mousewheel on one image
		_renderer.domElement.addEventListener('mousewheel', onDocumentMouseWheel, false);
		//mouse down
		_renderer.domElement.addEventListener('mousedown', onDocumentMouseDown, false);

		var container = document.getElementById(gldivId);
	}

	function onDocumentMouseWheel(event) {
		_camera.fov -= event.wheelDeltaY * 0.05;

		if (_camera.fov < 10.0) {
			_camera.fov = 10.0;
		}

		if (_camera.fov > 180.0) {
			_camera.fov = 180.0;
		}

		_camera.updateProjectionMatrix();

		render();
	}

	function onDocumentMouseDown(event) {

		//console.debug(_entities);
		//console.debug( _camera);
		//entities[0].children[0].material.color = new THREE.Color( 0xff0000 );
			
		event.preventDefault();

		//var container = document.getElementById("GLDiv");
		//var mouseX = (event.clientX / window.innerWidth) * 2 - 1;
		//var mouseY = -(event.clientY / window.innerHeight) * 2 + 1;
		
		//get innerWidth & innerHeight based on event containing window.
		var mouseX = (event.clientX / event.view.innerWidth) * 2 - 1;
		var mouseY = -(event.clientY / event.view.innerHeight) * 2 + 1;

		
		var vector = new THREE.Vector3(mouseX, mouseY, 0.5);

		var ray = new THREE.Ray(
			_camera.position,
			vector.subSelf(_camera.position).normalize());

		/*var vector = new THREE.Vector3(
			((event.clientX - container.offsetLeft) / _scene.WIDTH) * 2 - 1,
			-((event.clientY - container.offsetTop) / _scene.HEIGHT) * 2 + 1,
			0.5);*/

		_projector.unprojectVector(vector, _camera);

		var intersects = ray.intersectObjects(_entities);

		if (intersects.length > 0) {

			//SELECTED = intersects[0].object;

			alert("Intersect: " + intersects.length);

		}
	}

	function animate() {
		var id = requestAnimationFrame(animate);
		gid[divId] = id;
		_trackball.update();
		render();
		console.debug("animate");
	}

	function render() {
		_renderer.render(_scene, _camera);
			
	}
} //end of loadViewer
function loadSVGViewer(divId, svgFilePath)
{
	//home/david/testingFramwork/data/VisualizationArtifacts/results/vymrpruw
	readSVGFile(svgFilePath);
	function readSVGFile( fpath) {
		var url = fpath; //"readdata.svg";
		var result = "";
		$.ajax({
			url: url,
			type: 'GET',
			dataType: 'text',
			success: function (data) {
				result = data;
				console.debug("========success =================" + svgFilePath);
				var svgText = "<svg" +result.split("<svg")[1];
				$("#" + divId).append(result);
	
			},
			error : function (data){
				console.debug("========error =================" + svgFilePath);
				console.debug( data.statusText);
				$("#" + divId).append( "<p style='color:black'>" + data.statusText + "</p>");
			},
			async: false
		});
	}
} //end of loadViewer

//image need width and height
function loadImageViewer(divId, filepath, filename){
	//$("#" + divId).html("<IMG SRC='" + filepath + "' ALT='sometext' WIDTH = '424' HEIGHT='462'>")
	//$("<IMG SRC='" + filepath + "' ALT='" + filename + "' WIDTH = '500' HEIGHT='500'>").appendTo("#" + divId);
	$("<IMG SRC='" + filepath + "' ALT='" + filename + "' WIDTH = '100%'>").appendTo("#" + divId);
}
//youtube need with and height if 100% looks bad
function loadHTTP(divId, url) {
	var url = url.split("http:")[1];
	$('<iframe width="640" height="360"  src="'+ url + '" frameborder="0"></iframe>').appendTo("#" + divId);
	//$('<iframe width="640" height="360" src="//www.youtube.com/embed/_Pr_fpbAok8?feature=player_embedded" frameborder="0" allowfullscreen></iframe>').appendTo("#" + divId);
}

function loadMP4Video(divId, filepath) {
	$('<video id="videoid" width="100%" controls><source src = "'+ filepath + '" type ="video/mp4"></video>').appendTo("#" + divId);
}
function loadOtherVideo(divId, filepath) {
	$('<video id="videoid" width="100%" controls><source src = "'+ filepath + '"></video>').appendTo("#" + divId);
}





