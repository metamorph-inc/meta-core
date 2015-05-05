$vf = window.$vf || {};

$vf.EmbedVisualizer = function(src, afterThisElement, resource, popupURL) {

    trace('Embedding ['+src+']');

    var visualizerWrapperE = $('<div/>', {
        'class': 'visualizerWrapper'
    });

    var visualizerToolbarE = $('<div/>', {
        'class': 'visualizerToolbar'
    })

    visualizerWrapperE.append(visualizerToolbarE);

    var iframeE = $('<iframe/>', {
        src: src,
        'class': 'visualizerContainer'
    });

    var fullScreenButtonE = $('<button/>', {
        'class': 'newWindowButton',
        'text': 'Pop out into new window',
        'click': function() {
            var params  = 'width='+screen.width;
            params += ', height='+screen.height;
            params += ', top=0, left=0'
            params += ', fullscreen=yes';
            params += ', directories=no';
            params += ', location=no';
            params += ', menubar=no';
            params += ', resizable=no';
            params += ', scrollbars=no';
            params += ', status=no';
            params += ', toolbar=no';
            params += ', location=no';
            newwin=window.open(popupURL,'_blank', params);
            if (window.focus) {newwin.focus()}
        }
    });

    visualizerToolbarE.append(fullScreenButtonE);


    visualizerWrapperE.append(iframeE);

    var visualizerFooterE = $('<div/>', {
        'class': 'visualizerFooter'
    })

    visualizerFooterE.append($('<span class="bomLocation">Source: ['+resource+']</span>'))

    visualizerWrapperE.append(visualizerFooterE);

    afterThisElement.after(visualizerWrapperE);
}
