function isEmpty(el) {
    return !jQuery.trim(el.html())
}
jQuery(document).ready(function ($) {
    $("a[href*='doc_download']").each(function () {
        if (!isEmpty($(this))) {
            var sefurl = $(this).attr('href');
            // get doc id
            var docnamestart = sefurl.indexOf('doc_download') + 13; // doc_download plus the slash = 13 characters           
            var docalias = sefurl.substr(docnamestart); // now we have the whole sef document alias
            var docidend = docalias.indexOf('-');
            var docid = docalias.substr(0, docidend);
            // let's change the element ID to the docid so we can use it inside the function below
            $(this).attr('id', docid);
            var dataurl = '/index.php?option=com_microsemi&task=getdocflags&docid=' + docid;
            //{"isnew":false,"isupdated":true,"issecured":true}
            $.getJSON(dataurl, function (data) {
                // remove the BR tags inside and wrap with a DIV to preserve line breaks
                if ($("#"+docid).has('br').length) {
                    $("#"+docid).find('br').remove();
                    $("#"+docid).wrap('<div></div>');
                }
                if (data.isnew && !($('.doc_btn_new_' + docid).length)) {
                    //'&nbsp;<span class="oi" data-glyph="bolt" title="New" aria-hidden="true"></span>';
                    $("#"+docid).after('&nbsp;<span class="doc_btn_new_' + docid + '" title="New" aria-hidden="true" style="font-size:.7em;font-weight:normal;-webkit-border-radius: 5px;-moz-border-radius: 5px;border-radius: 5px;background-color:green;color:#FFF;padding: 0 3px 0 3px;">NEW</span>');
                }
                if (data.isupdated && !($('.doc_btn_updated_' + docid).length)) {
                    //'&nbsp;<span class="oi" data-glyph="star" title="Updated" aria-hidden="true"></span>';
                    $("#"+docid).after('&nbsp;<span class="doc_btn_updated_' + docid + '" title="Updated" aria-hidden="true" style="font-size:.7em;font-weight:normal;-webkit-border-radius: 5px;-moz-border-radius: 5px;border-radius: 5px;background-color:#2D7CE3;color:#FFF;padding: 0 3px 0 3px;">UPDATED</span>');
                }
                if (data.issecured && !($('.doc_btn_secured_' + docid).length)) {
                    //'&nbsp;<span class="oi" data-glyph="lock-locked" title="Secured" aria-hidden="true"></span>';
                    $("#"+docid).after('&nbsp;<span class="doc_btn_secured_' + docid + '" title="Secured" aria-hidden="true" ><img src="/components/com_microsemi/images/lock-locked.png" border=0 alt="Secured" width="10" height="10" /></span>');
                }
            });
        }
    });
});