$(function(){
	d3.json("pdf/outputs.json", function(JSONdata) {
	
		for (i = 0; i < JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric.length; i++)
		{
			var data;
			var xTitle = JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Name;
			//data = NormalDistribution(0, 1, 3, 500);
			//data = FourParameterBetaDistribution(0.5, 0.5, 80, 100, 500);
			//data = ThreeParameterGammaDistribution(1, 1, 0.1, 0, 10, 500);
			//data = PearsonTypeIVDistribution(1, 1, 1, 1, 1, -5, 5, 500);  //warning: NOT WORKING
			//data = InverseGammaDistribution(3, 1, 0, 3, 500);
			//data = FDistribution(1, 1, 1, 5, 500);
			//data = StudentsTDistribution(10, -4, 4, 500); 
			
			
			//What is the type of distribution of the dataset?
 			switch(JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Distribution.PearsonType)
			{
				case "0":
					//Normal
					var mean = +JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Moments.Mean,
						stdDev = Math.sqrt(+JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Moments.Variance);
					data = NormalDistribution(mean, stdDev, 3, 100);
					break;
				case "1":
					//Four-Parameter Beta
					var mean = +JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Moments.Mean,
						stdDev = Math.sqrt(+JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Moments.Variance);
					//data = FourParameterBetaDistribution(5, 1, 80, 100, 100);
					data = NormalDistribution(mean, stdDev, 3, 100);
					break;
				case "2":
					//Symmetric Four-Parameter Beta
					data = FourParameterBetaDistribution(5, 1, 80, 100, 100);
					break;
				case "3":
					//Three-Parameter Gamma
					data = ThreeParameterGammaDistribution(0.1, 0.1, 0.1, 0, 10, 100);
					break;
				case "4":
					//Pearson Type IV... It was decided to make these distributions NORMAL
					var mean = +JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Moments.Mean,
						stdDev = Math.sqrt(+JSONdata.Configurations.Configuration[0].PCCResults.PCCConfigMetric[i].Moments.Variance);
					data = NormalDistribution(mean, stdDev, 3, 100);
					break;
				case "5":
					//Inverse Gamma
					data = InverseGammaDistribution(3, 1, 0, 3, 250);
					break;
				case "6":
					//F
					data = FDistribution(1, 1, 1, 5, 100);
					break;
				case "7":
					//Student's T
					data = StudentsTDistribution(10, -4, 4, 250);
					break;
			} 
			
			//Formatting
			var widthpx = 309.8;
			var heightpx = 309.8;

			var margin = {top: 5, right: 5, bottom: 75, left: 5},
				width = widthpx - margin.left - margin.right,
				height = heightpx - margin.top - margin.bottom;

			var x = d3.scale.linear()
				.domain([data[0].x, data[data.length - 1].x])
				.range([0, width]);
			
			//What is the biggest and smallest Y value in the dataset?  
			//This is for proper scaling of the graphs
			var maxY = data[0].y;
			var minY = data[0].y;
			for (j= 1; j < data.length; j++)
			{
				if (data[j].y > maxY)
				{
					maxY = data[j].y;
				}
				if (data[j].y < minY)
				{
					minY = data[j].y;
				}
			}
			
			var y = d3.scale.linear()
				//.domain([data[0].y, data[(data.length - 1)/2].y])
				.domain([minY, maxY])
				.range([height, 0]);

			var xAxis = d3.svg.axis()
				.scale(x)
				.orient("bottom");
				//.attr("dy", ".35em")
				//.attr("text-anchor", "middle")
				//.text("TEST");

			var yAxis = d3.svg.axis()
				.scale(y)
				.orient("left");

			var line = d3.svg.line()
				.x(function(d) { return x(d.x); })
				.y(function(d) { return y(d.y); });

			var area = d3.svg.area()
				.x(line.x())
				.y1(line.y())
				.y0(y(0));
				
			var svg = d3.select("div#pdfchart" + i).append("svg") //each chart goes into its own DIV
				.datum(data)
				.attr("width", width + margin.left + margin.right)
				.attr("height", height + margin.top + margin.bottom)
				.append("g")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

			//this actaully draws the shading of the area below the line
			svg.append("path")
				.attr("class", "line")
				.attr("d", area);
				
			svg.append("g")
				.attr("class", "x axis")
				//.attr("stroke", "white")
				.attr("transform", "translate(0," + height + ")")
				.call(xAxis);

			//X-axis data label
			svg.append("g")
				.attr("class", "label")
				.attr("id", "xlabel")
				.append("text")
					.text(xTitle)
					.attr("x", 154.5)
					.attr("y", 260)
					.attr("text-anchor", "middle")
					.style("font-family", "Verdana")
					//.style("color", "black")
					.style("font-size", 10);

/* 			svg.append("g")
				.attr("class", "y axis")
				.call(yAxis);

			//add shading via path
			//svg.append("path")
			//	.attr("class", "line")
			//	.attr("d", line);
				
			//Y-axis data label
			svg.append("g")
				.attr("class", "label")
				.attr("id", "ylabel")
				.append("text")
					.text("Probabilty Density")
					.attr("x", 0)
					.attr("y", 0)
					.attr("text-anchor", "middle")
					.style("font-family", "Verdana")
					.style("color", "black")
					.style("font-size", 10)
					.attr("transform", "rotate(270 65 105)"); */
		}
	});
	
	function NormalDistribution(mean, sdev, numberOfSdev, numOfPoints) //Generates data for a Normal Distribution
	{
		//mean -- mean of the data
		//sdev -- standard deviation of the data
		//numberOfSdev -- number of standard deviations to take the data to (normally 3)
		//numOfPoints -- the number of points to calculate; fidelity
		
		var data = [];
		var x;
		var y;
		
		for (z = -numberOfSdev; z <= numberOfSdev; z += (2*numberOfSdev/numOfPoints)) //go through the Z score 
		{
			with (Math)
			{
				x = (mean + z*sdev);
				y = (1/(sdev* sqrt(2*PI)) * exp(-0.5* pow(z, 2)));
			}
			data.push({'x' : x, 'y' : y}); //store the dataset as an array of objects
		}
		
		return data;
	}

	function FourParameterBetaDistribution(alpha, beta, min, max, numOfPoints) //Generates data for a Four Parameter Beta Distribution
	{
		//Generate data set for a four-paremeter beta distribution
		
		//alpha -- shape parameter
		//beta -- shape parameter
		//min -- minimum of the data set as part of the 4 parameter beta dist
		//max -- maximum of the data set as part of the 4 parameter beta dist
		//numOfPoints -- the number of points to calculate; fidelity.  Needs to be an odd number.
		
		//Reference: http://en.wikipedia.org/wiki/Beta_function
		//Reference: http://en.wikipedia.org/wiki/Beta_distribution#Four_parameters
		
		//evaluate the beta function as defined by the gamma function.  See Reference 1
		var betaFunction = Math.exp(GammaLNFunction(alpha)) * Math.exp(GammaLNFunction(beta)) / Math.exp(GammaLNFunction(alpha + beta));
		
		var y;
		var data = [];
		
		for(x = min + (max - min)/numOfPoints; x < max - (max - min)/numOfPoints; x += (max - min)/numOfPoints)
		{
			y = (Math.pow(x - min, alpha - 1) * Math.pow(max - x, beta - 1)) / (betaFunction * Math.pow(max - min, alpha + beta - 1));
			data.push({'x' : x, 'y' : y}); //store the dataset as an array of objects
		}
		
		return data;
	};
	
	function ThreeParameterGammaDistribution(alpha, beta, gamma, min, max, numOfPoints) //Generates data for a Three Parameter Gamma Distribution
	{
		//This is a three parameter gamma function, also known as Pearson Type III
		
		//alpha -- shape parameter, alpha > 0
		//beta -- scale parameter, beta > 0
		//gamma -- location parameter.  gamma = 0 yeilds the standard 2 parameter gamma function
		//min -- minimum of the data set (used to feed x values into function)
		//max -- maximum of the data set (used to feed x values into function)
		//numOfPoints -- the number of points to calculate; fidelity
		
		//Reference: http://www.mathwave.com/articles/gamma_distribution.html
		
		var gammaFunction = Math.exp(GammaLNFunction(alpha));
		
		var y;
		var data = [];
		
		for (x = min + (max - min)/numOfPoints; x < max - (max - min)/numOfPoints; x += (max - min)/numOfPoints)
		{
			with(Math)
			{
				y = pow(x- gamma, alpha - 1) * exp(-(x-gamma)/beta)/(pow(beta,alpha)*gammaFunction);
			}
			
			data.push({'x' : x, 'y' : y}); //store the dataset as an array of objects
		}
		
		return data;
	};
	
	function PearsonTypeIVDistribution(a, a0, a1, a2, A, min, max, numOfPoints) //Generates data for a Pearson Type IV Distribution
	{
		//This function outputs the dataset for Pearson Type IV distribution
		//Reference: http://www.jstor.org/discover/10.2307/4356308?uid=3739616&uid=2129&uid=2&uid=70&uid=4&uid=3739256&sid=56185196243
		
		A = 1; //WHAT IS THIS? A NORMALIZING CONSTANT
		C1 = a0 - Math.pow(a1, 2) * Math.pow(4 * a2, -1);
		C2 = a1 * Math.pow(2 * a2, -1);
		
		var y;
		var data =[];
		
		for (w = min + (max - min)/numOfPoints; w < max - (max - min)/numOfPoints; w += (max - min)/numOfPoints)
		{
			with(Math)
			{
				y = A/pow((C1 + a2 * pow(w + C2, 2), 1/(2 * a2))) * exp((-a - C2)/sqrt(a2 * C1) * atan((w + C2)/sqrt(C1/a2)));
			}
			
			data.push({'x' : w, 'y' : y}); //store the dataset as an array of objects
		}
		return data;
	};
	
	function InverseGammaDistribution(alpha, beta, min, max, numOfPoints) //Generates data for a Inverse Gamma Distribution
	{
		//This function outputs the dataset for an inverse-gamma distribution
		
		//alpha -- shape parameter
		//beta -- scale parameter
		//min -- minimum of the data set (used to feed x values into function)
		//max -- maximum of the data set (used to feed x values into function)
		//numOfPoints -- the number of points to calculate; fidelity
		
		//Reference: http://en.wikipedia.org/wiki/Inverse-gamma_distribution
		
		var gammaFunction = Math.exp(GammaLNFunction(alpha));
		
		var y;
		var data = [];
				
		for (x = min + (max - min)/numOfPoints; x < max - (max - min)/numOfPoints; x += (max - min)/numOfPoints)
		{
			with(Math)
			{
				y = pow(beta, alpha)/gammaFunction * pow(x,-alpha - 1) * exp(-beta / x);
			}
			
			data.push({'x' : x, 'y' : y}); //store the dataset as an array of objects
		}
		
		return data;
	}
	
	function FDistribution(d1, d2, min, max, numOfPoints)  //Generates data for an F Distribution
	{
		//This function outputs the dataset for a two parameter F Distribution
		
		//d1 -- 1st parameter
		//d2 -- 2nd parameter
		//min -- minimum of the data set (used to feed x values into function)
		//max -- maximum of the data set (used to feed x values into function)
		//numOfPoints -- the number of points to calculate; fidelity
		
		//Reference: http://en.wikipedia.org/wiki/F-distribution
		
		var betaFunction = Math.exp(GammaLNFunction(d1/2)) * Math.exp(GammaLNFunction(d2/2)) / Math.exp(GammaLNFunction(d1/2 + d2/2));
		
		var y;
		var data = [];
		
		for (x = min + (max - min)/numOfPoints; x < max - (max - min)/numOfPoints; x += (max - min)/numOfPoints)
		{
			with(Math)
			{
				y = 1/betaFunction * pow(d1/d2, d1/2) * pow(x, d1/2 - 1) * pow(1 + d1/d2 * x, -(d1 + d2)/2);
			}
			
			data.push({'x' : x, 'y' : y}); //store the dataset as an array of objects
		}
		
		return data;
	};
	
	function StudentsTDistribution(v, min, max, numOfPoints) //Generates data for a Student's t-Distribution
	{
		//This function outputs the dataset for a Student's t-Distribution
		
		//v -- degress of freedom, real, v > 0
		//min -- minimum of the data set (used to feed x values into function)
		//max -- maximum of the data set (used to feed x values into function)
		//numOfPoints -- the number of points to calculate; fidelity
		
		//Reference: http://en.wikipedia.org/wiki/F-distribution
		
		var gammaFunction1 = Math.exp(GammaLNFunction((v+1)/2));
		var gammaFunction2 = Math.exp(GammaLNFunction(v/2));
		
		var y;
		var data = [];
		
		for (t = min; t < max; t += (max - min)/numOfPoints)
		{
			with(Math)
			{
				y = gammaFunction1/(sqrt(v*PI)*gammaFunction2) * pow(1 + pow(t,2)/v, -(v+1)/2);
			}
			
			data.push({'x' : t, 'y' : y}); //store the dataset as an array of objects
		}
		
		return data;
	};
	
	function GammaLNFunction(Z) // Gamma LN
	{
		//Returns the natural log of the Gamma Function
		//This function was found on http://www.math.ucla.edu/~tom/distributions/gamma.html
		with (Math) 
		{
			var S=1+76.18009173/Z-86.50532033/(Z+1)+24.01409822/(Z+2)-1.231739516/(Z+3)+.00120858003/(Z+4)-.00000536382/(Z+5);
			var LG= (Z-.5)*log(Z+4.5)-(Z+4.5)+log(S*2.50662827465);
		}
		return LG;
	};
});