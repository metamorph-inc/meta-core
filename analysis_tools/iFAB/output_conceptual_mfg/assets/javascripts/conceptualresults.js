$(function() {
  // Get Conceptual Manufacturability Results
  $.ajax({url: "conceptual_cost.json", dataType: "json",
  error: function() {
    var resultsWrapper = $('#results-wrapper');

    if(/chrom(e|ium)/.test(navigator.userAgent.toLowerCase())) {
      resultsWrapper.append('<b>Attention Chrome Users:</b> Chrome has a "security feature" that prevents it from loading the result file.  Please open this webpage in Firefox or Internet Explorer.');
    } else {
      resultsWrapper.append("Failed to load manufacturability results.  Please check your error logs to identify the issue and, if necessary, submit a bug report to VehicleForge.");
    }
  },
  success: function(jsonData) {
    // Define The Page
    var resultsWrapper = $('#results-wrapper');

    // Create The Page Header
    resultsWrapper.append('<h1>Conceptual Manufacturability Analysis</h1>');

      var partError = false;
      var problemParts = [];

      $.each(jsonData.components.detail, function(index, part) {
        if (part.cost == "no manufacturing model" || part.leadtime == "no manufacturing model") {
          partError = true;
          problemParts.push(part.name);
        }
      });

      // Add Summary Data
      resultsWrapper.append('<h2>Summary</h2></br>');
      var summaryTable = $('<table />').addClass('table table-striped table-hover');
      summaryTable.append('<tr><td width="20%">Total Cost:</td><td>$' + jsonData.totals['total cost (dollars)'].toFixed(2) + '</td></tr>');
      summaryTable.append('<tr><td width="20%">Total Time:</td><td>' + jsonData.totals['total lead time (days)'].toFixed(1) + ' days</td></tr>');
      if (partError) {
        summaryTable.append('<tr><td width="20%">Error:</td><td><font color="red">Unable to find manufacturing models for one or more parts.  The reported costs may be incorrect.  Conceptual manufacturability analysis is not able to cost custom parts.  Please use the detailed manufacturability testbench to analyze such designs.</font></td></tr>');
      }
      resultsWrapper.append(summaryTable);

      if (jsonData.hull['Total Mass']) {
        resultsWrapper.append('<h2>Hull Cost Analysis</h2>');
        var hullTable = $('<table />').addClass('table table-striped table-hover');
        hullTable.append('<tr><td width="20%">Total Mass:</td><td>' + addCommas(jsonData.hull['Total Mass'].toFixed(1)) + " kgs</td></tr>");
        hullTable.append('<tr><td width="20%">Number of Plates:</td><td>' + jsonData.hull.plates.length + '</td></tr>');
        hullTable.append('<tr><td width="20%">Total Cost:</td><td>$' + addCommas(jsonData.hull['Total Cost'].toFixed(2)) + "</td></tr>");
        hullTable.append('<tr><td width="20%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Material Cost:</td><td>$' + addCommas(jsonData.hull['Material Cost'].toFixed(2)) + "</td></tr>");
        hullTable.append('<tr><td width="20%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Build Cost:</td><td>$' + addCommas(jsonData.hull['Build Cost'].toFixed(2)) + "</td></tr>");
        hullTable.append('<tr><td width="20%">Total Time:</td><td>' + jsonData.hull['Total Time'].toFixed(1) + " days</td></tr>");
        hullTable.append('<tr><td width="20%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Material Lead Time:</td><td>' + jsonData.hull['Material Lead Time'].toFixed(1) + " days</td></tr>");
        hullTable.append('<tr><td width="20%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Build Time:</td><td>' + jsonData.hull['Build Time'].toFixed(1) + " days</td></tr>");
        resultsWrapper.append(hullTable);
      }

      resultsWrapper.append('<h2>Purchased Parts</h2>');
      var componentsTable = $('<table />').addClass('table table-striped table-hover');
      componentsTable.append('<tr><td width="20%">Number of Parts:</td><td>' + jsonData.components.detail.length + '</td></tr>');
      componentsTable.append('<tr><td width="20%">Total Cost:</td><td>$' + addCommas(jsonData.components.cost.toFixed(2)) + "</td></tr>");
      componentsTable.append('<tr><td width="20%">Total Time:</td><td>' + jsonData.components.leadtime.toFixed(1) + " days</td></tr>");

      if (partError) {
        componentsTable.append('<tr><td width="20%">Problems:</td><td>No procurement/manufacturing information could be located for the following parts: ' + problemParts.join(", ") + "</td></tr>");
      }

      resultsWrapper.append(componentsTable);
  }});
});

function addCommas(nStr)
{
  nStr += '';
  x = nStr.split('.');
  x1 = x[0];
  x2 = x.length > 1 ? '.' + x[1] : '';
  var rgx = /(\d+)(\d{3})/;
  while (rgx.test(x1)) {
    x1 = x1.replace(rgx, '$1' + ',' + '$2');
  }
  return x1 + x2;
}