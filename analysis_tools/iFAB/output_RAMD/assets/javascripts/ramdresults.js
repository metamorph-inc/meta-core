$(function() {
  // Get Structural Completeness Results
  $.ajax({url: "ramd_results.json", dataType: "json",
  error: function() {
    var resultsWrapper = $('#results-wrapper');

    if(/chrom(e|ium)/.test(navigator.userAgent.toLowerCase())) {
      resultsWrapper.append('<b>Attention Chrome Users:</b> Chrome has a "security feature" that prevents it from loading the result file.  Please open this webpage in Firefox or Internet Explorer.');
    } else {
      resultsWrapper.append("Failed to load structural completeness results.  Please check your error logs to identify the issue and, if necessary, submit a bug report to VehicleForge.");
    }
  },
  success: function(jsonData) {
    // Define The Page
    var resultsWrapper = $('#results-wrapper');

    // Create The Page Header
    resultsWrapper.append('<h1>RAMD Analysis</h1>');

      // Add Summary / Help Info
      resultsWrapper.append('<h2>About</h2>');
      resultsWrapper.append('<p style="margin: 10px">The RAMD testbench analyzes a vehicle design for reliability, accessibility, and maintainability.  The overall RAMD score is the aggregate of these three individual components, detailed below.</p>');
      resultsWrapper.append('<p style="margin: 10px"><b>Reliability</b> - Reliability measures the probability of failure for the vehicle.  If the vehicle contains more parts, the probability that at least one part breaks down increases.  Reliability estimates this probability.  A reliability score of 100 is ideal, which indicates the vehicle has a low probability of failure.');
      resultsWrapper.append('<p style="margin: 10px"><b>Accessibility</b> - Ease-of-access to critical components, such as the engine, is an important aspect of vehicle design.  If the area around a critical component is cluttered, it becomes more difficult to access the component.  An accessibility score of 100 is ideal, which indicates the component is easily accessible.');
      resultsWrapper.append('<p style="margin: 10px"><b>Maintainability</b> - If critical components break down in the field, such as the power pack, it is necessary to quickly replace the component.  Maintainability ensures the power pack can be removed from the vehicle and a new power pack installed.  A maintainability score of 100 is ideal, which indicates the power pack can be replaced.  The output below contains additional details on the maintainability calculations.');

      // Add Summary Data
      resultsWrapper.append('<h2>Summary</h2>');
      var summaryTable = $('<table />').addClass('table table-striped table-hover');
      summaryTable.append('<tr><td width="20%"><b>Overall RAMD Score:</b></td><td><b>' + jsonData["RAMD"].toFixed(1) + '</b></td></tr>');
      summaryTable.append('<tr><td width="20%">Reliability:</td><td>' + jsonData["reliability"].toFixed(1) + '</td></tr>');
      summaryTable.append('<tr><td width="20%">Accessibility:</td><td>' + jsonData["accessibility"].toFixed(1) + '</td></tr>');
      summaryTable.append('<tr><td width="20%">Maintainability:</td><td>' + jsonData["maintainability"].toFixed(1) + '</td></tr>');
      resultsWrapper.append(summaryTable);

      resultsWrapper.append('<h2>Maintainability Details</h2>');
      var detailsTable = $('<table />').addClass('table table-striped table-hover');
      detailsTable.append('<tr><td width="20%">Successful:</td><td>' + (jsonData["maintainability_success"] ? "Yes" : "No") + '</td></tr>');

      if (jsonData["maintainability_success"]) {
        screenshots = '<tr><td width="20%">Screenshots:</td><td>The images below show the path required to remove the power pack (engine, transmission, etc.) from the vehicle:<br />';

        $.each(jsonData["maintainability_screenshots"], function(index, image) {
          screenshots += '<img style="margin: 2px" width="40%" src="' + image + '"/>';
        });

        detailsTable.append(screenshots);
      } else {
        if (jsonData["maintainability_message"]) {
          detailsTable.append('<tr><td width="20%">Message:</td><td>' + jsonData["maintainability_message"] + '</td></tr>');
        } else {
          detailsTable.append('<tr><td width="20%">Message:</td><td>' + "Maintainability analysis failed for an unknown reason" + '</td></tr>');
        }
      }

      resultsWrapper.append(detailsTable);

      $('img.tip').qtip({
        style: {classes: 'qtip-bootstrap qtip-shadow'}
      });
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