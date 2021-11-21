var $j = jQuery.noConflict();
jQuery(document).ready(function() {

    if(jQuery('[name="docType[]"]:checked').length == 0 && jQuery('[name="docFeat[]"]:checked').length == 0){
             jQuery( ".docfeachk" ).prop( "checked", true );
            jQuery( ".doctypechk" ).prop( "checked", true );
            jQuery( "#docSel" ).prop( "checked", true );
            jQuery( "#feaSel" ).prop( "checked", true );
    }
    jQuery('#feaSel').click(function(event) {  //on click 
       if(this.checked) { // check select status
           jQuery('.docfeachk').each(function() { //loop through each checkbox
               this.checked = true;  //select all checkboxes with class "checkbox1"               
           });
       }else{
           jQuery('.docfeachk').each(function() { //loop through each checkbox
               this.checked = false; //deselect all checkboxes with class "checkbox1"                       
           });         
       }
   });
   jQuery('#docSel').click(function(event) {  //on click 
       if(this.checked) { // check select status
           jQuery('.doctypechk').each(function() { //loop through each checkbox
               this.checked = true;  //select all checkboxes with class "checkbox1"               
           });
       }else{
          jQuery('.doctypechk').each(function() { //loop through each checkbox
               this.checked = false; //deselect all checkboxes with class "checkbox1"                       
           });         
       }
   });
   
   jQuery('#alldoctype').change(function() {  //on click 
       if(this.checked) { // check select status
           jQuery('.doctypechk').attr('disabled','disabled');
           jQuery('#docSel').attr('disabled','disabled');
           
       }else{
           jQuery('.doctypechk').removeAttr('disabled');
           jQuery('#docSel').removeAttr('disabled');
       }
   });
   
});