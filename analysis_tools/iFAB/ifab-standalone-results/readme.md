# Setup

On the forge this visualizer runs with the following loaded:

## from http://jquery.com/ 

- jquery.1.7.2.min.js
- jquery-ui.1.10.3.js

## from http://www.flotcharts.org/

- jquery.flot.js
- jquery.flot.selection.js
- jquery.flot.pie.js

## included

- ifabresults.css
- ifabresults.js


# required HTML:

    <div id="results_container"></div>
    <script type="text/javascript">
        var RESOURCE_URL = __URL_OF_JSON_FILE__;
    </script>


# Caveat:
This requires XMLHTTPRequest capabilities in the browser. If this has to be 
rendered from a local machine without these capabilities then the JSON file
to be visualized must be injected into the `ifabresults.js` file as the value of
the `jsonData` variable (set at line 367) before loading the page.
