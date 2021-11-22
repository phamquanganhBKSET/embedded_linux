jQuery.noConflict();
if(typeof(BTLJ)=='undefined') var BTLJ = jQuery;
if(typeof(btTimeOut)=='undefined') var btTimeOut;
if(typeof(requireRemove)=='undefined') var requireRemove = true;

//var autoPos = !('ontouchstart' in window);
BTLJ(document).ready(function() {

	BTLJ('#btl-content').appendTo('body');
	BTLJ(".btl-input #jform_profile_aboutme").attr("cols",21);
	BTLJ('.bt-scroll .btl-buttonsubmit').click(function(){
		setTimeout(function(){
			if(BTLJ("#btl-registration-error").is(':visible')){
				BTLJ('.bt-scroll').data('jsp').scrollToY(0,true);
			}else{
				var position = BTLJ('.bt-scroll').find('.invalid:first').position();
				if(position) BTLJ('.bt-scroll').data('jsp').scrollToY(position.top-15,true);
			}
		},20);
	})
	//SET POSITION
	if(BTLJ('.btl-dropdown').length){
		setFPosition();
		BTLJ(window).resize(function(){
			setFPosition();
		})
	}

	BTLJ(btlOpt.LOGIN_TAGS).addClass("btl-modal");
	if(btlOpt.REGISTER_TAGS != ''){
		BTLJ(btlOpt.REGISTER_TAGS).addClass("btl-modal");
	}

	// Login event
	var elements = '#btl-panel-login';
	if (btlOpt.LOGIN_TAGS) elements += ', ' + btlOpt.LOGIN_TAGS;
	if (btlOpt.MOUSE_EVENT =='click'){
		BTLJ(elements).click(function (event) {
				showLoginForm();
				event.preventDefault();
		});
	}else{
		BTLJ(elements).hover(function () {
				showLoginForm();
		},function(){});
	}

	// Registration/Profile event
	elements = '#btl-panel-registration';
	if (btlOpt.REGISTER_TAGS) elements += ', ' + btlOpt.REGISTER_TAGS;
	if (btlOpt.MOUSE_EVENT =='click'){
		BTLJ(elements).click(function (event) {
			showRegistrationForm();
			event.preventDefault();
		});
		BTLJ("#btl-panel-profile").click(function(event){
			showProfile();
			event.preventDefault();
		});
	}else{
		BTLJ(elements).hover(function () {
				if(!BTLJ("#btl-integrated").length){
					showRegistrationForm();
				}
		},function(){});
		BTLJ("#btl-panel-profile").hover(function () {
				showProfile();
		},function(){});
	}
	BTLJ('#register-link a').click(function (event) {
			if(BTLJ('.btl-modal').length){
				BTLJ.modal.close();setTimeout("showRegistrationForm();",1000);
			}
			else{
				showRegistrationForm();
			}
			event.preventDefault();
	});

	// Close form
	BTLJ(document).click(function(event){
		if(requireRemove && event.which == 1) btTimeOut = setTimeout('BTLJ("#btl-content > div").slideUp();BTLJ(".btl-panel span").removeClass("active");',10);
		requireRemove =true;
	})
	BTLJ(".btl-content-block").click(function(){requireRemove =false;});
	BTLJ(".btl-panel span").click(function(){requireRemove =false;});

	// Modify iframe
	BTLJ('#btl-iframe').load(function (){
		//edit action form
		oldAction=BTLJ('#btl-iframe').contents().find('form').attr("action");
		if(oldAction!=null){
			if(oldAction.search("tmpl=component")==-1){
				if(BTLJ('#btl-iframe').contents().find('form').attr("action").indexOf('?')!=-1){
					BTLJ('#btl-iframe').contents().find('form').attr("action",oldAction+"&tmpl=component");
				}
				else{
					BTLJ('#btl-iframe').contents().find('form').attr("action",oldAction+"?tmpl=component");
				}
			}
		}
	});

	//reload captcha click event
	BTLJ('span#btl-captcha-reload').click(function(){
		BTLJ.ajax({
						type: "post",
						url: btlOpt.BT_AJAX,
						data: 'bttask=reload_captcha',
						success: function(html){
							BTLJ('#recaptcha img').attr('src', html);
						}
					});
	});

});

function setFPosition(){
	
}

// SHOW LOGIN FORM
function showLoginForm(){
	BTLJ('.btl-panel span').removeClass("active");
	var el = '#btl-panel-login';
	if (btlOpt.LOGIN_TAGS) el += ', ' + btlOpt.LOGIN_TAGS;
	BTLJ.modal.close();
	var containerWidth = 0;
	var containerHeight = 0;
	containerHeight = 505;
	containerWidth = 550;

	if(containerWidth>BTLJ(window).width()){
		containerWidth = BTLJ(window).width()-50;
	}
	if(BTLJ(el).hasClass("btl-modal")){
		BTLJ(el).addClass("active");
		BTLJ("#btl-content > div").slideUp();
		BTLJ("#btl-content-login").modal({
			overlayClose:true,
			persist :true,
			autoPosition:true,
			fixed: BTLJ(window).width()>500,
			onOpen: function (dialog) {
				dialog.overlay.fadeIn();
				dialog.container.show();
				dialog.data.show();
			},
			onClose: function (dialog) {
				dialog.overlay.fadeOut(function () {
					dialog.container.hide();
					dialog.data.hide();
					BTLJ.modal.close();
					BTLJ('.btl-panel span').removeClass("active");
				});
			},
			containerCss:{
				height:containerHeight,
				width:containerWidth
			}
		})
	}
	else
	{
		setFPosition();
		BTLJ("#btl-content > div").each(function(){
			if(this.id=="btl-content-login")
			{
				if(BTLJ(this).is(":hidden")){
					BTLJ(el).addClass("active");
					BTLJ(this).slideDown();
					}
				else{
					BTLJ(this).slideUp();
					BTLJ(el).removeClass("active");
				}

			}
			else{
				if(BTLJ(this).is(":visible")){
					BTLJ(this).slideUp();
					BTLJ('#btl-panel-registration').removeClass("active");
				}
			}

		})
	}
}

// SHOW REGISTRATION FORM
function showRegistrationForm(){
	if(BTLJ("#btl-integrated").length){
		window.location.href=BTLJ("#btl-integrated").val();
		return;
	}
	BTLJ('.btl-panel span').removeClass("active");
	BTLJ.modal.close();
	var el = '#btl-panel-registration';
	var containerWidth = 0;
	var containerHeight = 0;
	containerHeight = "auto";
	containerWidth = "auto";
	if(containerWidth>BTLJ(window).width()){
		containerWidth = BTLJ(window).width();
	}
	if(BTLJ(el).hasClass("btl-modal")){
		BTLJ(el).addClass("active");
		BTLJ("#btl-content > div").slideUp();
		BTLJ("#btl-content-registration").modal({
			overlayClose:true,
			persist :true,
			autoPosition:true,
			fixed: BTLJ(window).width()>500,
			onOpen: function (dialog) {
				dialog.overlay.fadeIn();
				dialog.container.show();
				dialog.data.show();
			},
			onClose: function (dialog) {
				dialog.overlay.fadeOut(function () {
					dialog.container.hide();
					dialog.data.hide();
					BTLJ.modal.close();
					BTLJ('.btl-panel span').removeClass("active");
				});
			},
			containerCss:{
				height:containerHeight,
				width:containerWidth
			}
		})
	}
	else
	{
		setFPosition();
		BTLJ("#btl-content > div").each(function(){
			if(this.id=="btl-content-registration")
			{
				if(BTLJ(this).is(":hidden")){
					BTLJ(el).addClass("active");
					BTLJ(this).slideDown();
					}
				else{
					BTLJ(this).slideUp();
					BTLJ(el).removeClass("active");
					}
			}
			else{
				if(BTLJ(this).is(":visible")){
					BTLJ(this).slideUp();
					BTLJ('#btl-panel-login').removeClass("active");
				}
			}

		})
	}
}

// SHOW PROFILE (LOGGED MODULES)
function showProfile(){
	setFPosition();
	var el = '#btl-panel-profile';
	BTLJ("#btl-content > div").each(function(){
		if(this.id=="btl-content-profile")
		{
			if(BTLJ(this).is(":hidden")){
				BTLJ(el).addClass("active");
				BTLJ(this).slideDown();
				}
			else{
				BTLJ(this).slideUp();
				BTLJ('.btl-panel span').removeClass("active");
			}
		}
		else{
			if(BTLJ(this).is(":visible")){
				BTLJ(this).slideUp();
				BTLJ('.btl-panel span').removeClass("active");
			}
		}

	})
}

// AJAX REGISTRATION
function registerAjax(){
	BTLJ("#btl-registration-error").hide();
	 BTLJ(".btl-error-detail").hide();
	if(BTLJ("#btl-input-name").val()==""){
		BTLJ("#btl-registration-error").html(Joomla.JText._('REQUIRED_NAME')).show();
		BTLJ("#btl-input-name").focus();
		return false;
	}
	if(BTLJ("#btl-input-username1").val()==""){
		BTLJ("#btl-registration-error").html(Joomla.JText._('REQUIRED_USERNAME')).show();
		BTLJ("#btl-input-username1").focus();
		return false;
	}
	if(BTLJ("#btl-input-password1").val()==""){
		BTLJ("#btl-registration-error").html(Joomla.JText._('REQUIRED_PASSWORD')).show();
		BTLJ("#btl-input-password1").focus();
		return false;
	}
	if(BTLJ("#btl-input-password2").val()==""){
		BTLJ("#btl-registration-error").html(Joomla.JText._('REQUIRED_VERIFY_PASSWORD')).show();
		BTLJ("#btl-input-password2").focus();
		return false;
	}
	if(BTLJ("#btl-input-password2").val()!=BTLJ("#btl-input-password1").val()){
		BTLJ("#btl-registration-error").html(Joomla.JText._('PASSWORD_NOT_MATCH')).show();
		BTLJ("#btl-input-password2").focus().select();
		BTLJ("#btl-registration-error").show();
		return false;
	}
	if(BTLJ("#btl-input-email1").val()==""){
		BTLJ("#btl-registration-error").html(Joomla.JText._('REQUIRED_EMAIL')).show();
		BTLJ("#btl-input-email1").focus();
		return false;
	}
	var emailRegExp = /^[_a-zA-Z0-9-]+(\.[_a-zA-Z0-9-]+)*@[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)*(\.([a-zA-Z]){2,4})$/;
	if(!emailRegExp.test(BTLJ("#btl-input-email1").val())){
		BTLJ("#btl-registration-error").html(Joomla.JText._('EMAIL_INVALID')).show();
		BTLJ("#btl-input-email1").focus().select();
		return false;
	}
	if(BTLJ("#btl-input-email2").val()==""){
		BTLJ("#btl-registration-error").html(Joomla.JText._('REQUIRED_VERIFY_EMAIL')).show();
		BTLJ("#btl-input-email2").focus().select();
		return false;
	}
	if(BTLJ("#btl-input-email1").val()!=BTLJ("#btl-input-email2").val()){
		BTLJ("#btl-registration-error").html(Joomla.JText._('EMAIL_NOT_MATCH')).show();;
		BTLJ("#btl-input-email2").focus().select();
		return false;
	}
	if(btlOpt.RECAPTCHA =="recaptcha"){
		if(BTLJ('#recaptcha_response_field').length && BTLJ('#recaptcha_response_field').val()==''){
			BTLJ("#btl-registration-error").html(Joomla.JText._('CAPTCHA_REQUIRED')).show();
			BTLJ('#recaptcha_response_field').focus();
			return false;
		}
	}else if(btlOpt.RECAPTCHA =="2"){
		if(BTLJ('#btl-captcha').length && BTLJ('#btl-captcha').val()==''){
			BTLJ("#btl-registration-error").html(Joomla.JText._('CAPTCHA_REQUIRED')).show();
			BTLJ('#btl-captcha').focus();
			return false;
		}
	}

	var token = BTLJ('.btl-buttonsubmit input:last').attr("name");
	var value_token = encodeURIComponent(BTLJ('.btl-buttonsubmit input:last').val());
	var datasubmit= "bttask=register&name="+encodeURIComponent(BTLJ("#btl-input-name").val())
			+"&username="+encodeURIComponent(BTLJ("#btl-input-username1").val())
			+"&passwd1=" + encodeURIComponent(BTLJ("#btl-input-password1").val())
			+"&passwd2=" + encodeURIComponent(BTLJ("#btl-input-password2").val())
			+"&email1=" + encodeURIComponent(BTLJ("#btl-input-email1").val())
			+"&email2=" + encodeURIComponent(BTLJ("#btl-input-email2").val())
			+ "&"+token+"="+value_token;
	if(btlOpt.RECAPTCHA =="recaptcha"){
		datasubmit  += "&recaptcha=yes&recaptcha_response_field="+ encodeURIComponent(BTLJ("#recaptcha_response_field").val())
					+"&recaptcha_challenge_field="+encodeURIComponent(BTLJ("#recaptcha_challenge_field").val());
	}else if(btlOpt.RECAPTCHA =="2"){
		datasubmit  += "&recaptcha=yes&btl_captcha="+ encodeURIComponent(BTLJ("#btl-captcha").val());
	}

	BTLJ.ajax({
		   type: "POST",
		   beforeSend:function(){
			   BTLJ("#btl-register-in-process").show();
		   },
		   url: btlOpt.BT_AJAX,
		   data: datasubmit,
		   success: function(html){
			   //if html contain "Registration failed" is register fail
			  BTLJ("#btl-register-in-process").hide();
			  if(html.indexOf('$error$')!= -1){
				  BTLJ("#btl-registration-error").html(html.replace('$error$',''));
				  BTLJ("#btl-registration-error").show();
				  if(btlOpt.RECAPTCHA =="recaptcha"){
					  Recaptcha.reload();
				  }else if(btlOpt.RECAPTCHA =="2"){
					BTLJ.ajax({
						type: "post",
						url: btlOpt.BT_AJAX,
						data: 'bttask=reload_captcha',
						success: function(html){
							BTLJ('#recaptcha img').attr('src', html);
						}
					});
				  }

			   }else{
				   BTLJ(".btl-formregistration").children("div").hide();
				   BTLJ("#btl-success").html(html);
				   BTLJ("#btl-success").show();
				   setTimeout(function() {window.location.reload();},7000);

			   }
		   },
		   error: function (XMLHttpRequest, textStatus, errorThrown) {
				alert(textStatus + ': Ajax request failed');
		   }
		});
		return false;
}

// AJAX LOGIN
function loginAjax(){
	if(BTLJ("#btl-input-username").val()=="") {
		showLoginError(Joomla.JText._('REQUIRED_USERNAME'));
		return false;
	}
	if(BTLJ("#btl-input-password").val()==""){
		showLoginError(Joomla.JText._('REQUIRED_PASSWORD'));
		return false;
	}
	var token = BTLJ('.btl-buttonsubmit input:last').attr("name");
	var value_token = encodeURIComponent(BTLJ('.btl-buttonsubmit input:last').val());
	var datasubmit= "bttask=login&username="+encodeURIComponent(BTLJ("#btl-input-username").val())
	+"&passwd=" + encodeURIComponent(BTLJ("#btl-input-password").val())
	+ "&"+token+"="+value_token
	+"&return="+ encodeURIComponent(BTLJ("#btl-return").val());

	if(BTLJ("#btl-checkbox-remember").is(":checked")){
		datasubmit += '&remember=yes';
	}

	BTLJ.ajax({
	   type: "POST",
	   beforeSend:function(){
		   BTLJ("#btl-login-in-process").show();
		   BTLJ("#btl-login-in-process").css('height',BTLJ('#btl-content-login').outerHeight()+'px');

	   },
	   url: btlOpt.BT_AJAX,
	   data: datasubmit,
	   success: function (html, textstatus, xhrReq){
		  if(html == "1" || html == 1){
			   window.location.href=btlOpt.BT_RETURN;
		   }else{
			   if(html.indexOf('</head>')==-1){
				   showLoginError(Joomla.JText._('E_LOGIN_AUTHENTICATE'));
				}
				else
				{
					if(html.indexOf('btl-panel-profile')==-1){
						showLoginError('Another plugin has redirected the page on login, Please check your plugins system');
					}
					else
					{
						window.location.href=btlOpt.BT_RETURN;
					}
				}
		   }
	   },
	   error: function (XMLHttpRequest, textStatus, errorThrown) {
			alert(textStatus + ': Ajax request failed!');
	   }
	});
	return false;
}
function showLoginError(notice,reload){
	BTLJ("#btl-login-in-process").hide();
	BTLJ("#btl-login-error").html(notice);
	BTLJ("#btl-login-error").show();
	if(reload){
		setTimeout(function() {window.location.reload();},5000);
	}
}
