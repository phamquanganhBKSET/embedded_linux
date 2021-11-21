jQuery(document).ready(function () {
    jQuery('.pmc_api_docs_container').each(function () {
        var currentTag = jQuery(this);
        var pmcAPI = 'https://pmcs.com/cgi-bin/docBuilderDocTable_mscc.pl';
		  //get the tag code from the div
        var requestTag = currentTag.data('apitag');
           // create the post to the API
        jQuery.ajax({
            type: "POST",
            url: pmcAPI,
			data: {prod: requestTag},
			success: function (data) {
                currentTag.html(data);
			 },
            dataType: 'html'});
    });

});
