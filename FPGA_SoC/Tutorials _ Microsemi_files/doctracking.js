// Google Analytics
(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
})(window,document,'script','//www.google-analytics.com/analytics.js','ga');
 
ga('create', 'UA-20085-2', 'auto');  // Replace with your property ID.
//ga('send', 'pageview');

// End Google Analytics
jQuery(document).ready(function () {
    //jQuery("a[href*='doc_download']").on('click', function () {
    jQuery(document).on('click',"a[href*='doc_download']", function () {
        var sefurl = jQuery(this).attr('href');
        // get doc id
        var docnamestart = sefurl.indexOf('doc_download') + 13; // doc_download plus the slash = 13 characters           
        var docalias = sefurl.substr(docnamestart); // now we have the whole sef document alias
        var docidend = docalias.indexOf('-');
        var docid = docalias.substr(0, docidend);
        var doctitle = docalias.replace(/\-/g, ' ').substr(docidend + 1).toUpperCase();
        var docname = docid + '|' + doctitle;
        var caturl = '/index.php?option=com_microsemi&task=getcatid&format=raw&layout=getcatid&docid=' + docid;

        jQuery.get(caturl, function (catid) {
            //ga('send', 'event', 'Documents', 'Downloads', docname, parseInt(catid));
            //_gaq.push(['_trackEvent', 'Documents', 'Download', docname, parseInt(catid)]);
			ga('send', {
			    hitType: 'event', 
			    eventCategory: 'Documents',
			    eventAction: 'Download',
			    eventLabel: docname,
			    eventValue: parseInt(catid)
			    /* test event callback
			    hitCallback : function () {
			      alert("Event received");
			   }
			   */
			});
        });
    });
});
