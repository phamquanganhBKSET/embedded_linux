/**
 *
 * JQUERY EU COOKIE LAW POPUPS
 * version 1.1.1
 *
 * Code on Github:
 * https://github.com/wimagguc/jquery-eu-cookie-law-popup
 *
 * To see a live demo, go to:
 * http://www.wimagguc.com/2018/05/gdpr-compliance-with-the-jquery-eu-cookie-law-plugin/
 *
 * by Richard Dancsi
 * http://www.wimagguc.com/
 *
 */

(function($) {

// for ie9 doesn't support debug console >>>
if (!window.console) window.console = {};
if (!window.console.log) window.console.log = function () { };
// ^^^

$.fn.euCookieLawPopup = (function() {

	var _self = this;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// PARAMETERS (MODIFY THIS PART) //////////////////////////////////////////////////////////////
	_self.params = {
		cookiePolicyUrl : '/company/microsemi-cookie-statement',
		popupPosition : 'top',
		colorStyle : 'default',
		compactStyle : false,
		popupTitle : '',
		popupText : 'This website uses cookies for analytics, personalization, and other purposes. Click to learn more. By continuing to browse, you agree to our use of cookies as described in our Cookies Statement.',
		buttonContinueTitle : 'OK',
		buttonLearnmoreTitle : 'Learn&nbsp;more',
		buttonLearnmoreOpenInNewWindow : true,
		agreementExpiresInDays : 30,
		autoAcceptCookiePolicy : false,
		htmlMarkup : null
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// VARIABLES USED BY THE FUNCTION (DON'T MODIFY THIS PART) ////////////////////////////////////
	_self.vars = {
		INITIALISED : false,
		HTML_MARKUP : null,
		COOKIE_NAME : 'EU_COOKIE_LAW_CONSENT'
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// PRIVATE FUNCTIONS FOR MANIPULATING DATA ////////////////////////////////////////////////////

	// Overwrite default parameters if any of those is present
	var parseParameters = function(object, markup, settings) {

		if (object) {
			var className = $(object).attr('class') ? $(object).attr('class') : '';
			if (className.indexOf('cookiepopup-top') > -1) {
				_self.params.popupPosition = 'top';
			}
			else if (className.indexOf('cookiepopup-fixedtop') > -1) {
				_self.params.popupPosition = 'fixedtop';
			}
			else if (className.indexOf('cookiepopup-bottomright') > -1) {
				_self.params.popupPosition = 'bottomright';
			}
			else if (className.indexOf('cookiepopup-bottomleft') > -1) {
				_self.params.popupPosition = 'bottomleft';
			}
			else if (className.indexOf('cookiepopup-bottom') > -1) {
				_self.params.popupPosition = 'bottom';
			}
			else if (className.indexOf('cookiepopup-block') > -1) {
				_self.params.popupPosition = 'block';
			}
			if (className.indexOf('cookiepopup-color-default') > -1) {
				_self.params.colorStyle = 'default';
			}
			else if (className.indexOf('cookiepopup-color-inverse') > -1) {
				_self.params.colorStyle = 'inverse';
			}
			if (className.indexOf('cookiepopup-style-compact') > -1) {
				_self.params.compactStyle = true;
			}
		}

		if (markup) {
			_self.params.htmlMarkup = markup;
		}

		if (settings) {
			if (typeof settings.cookiePolicyUrl !== 'undefined') {
				_self.params.cookiePolicyUrl = settings.cookiePolicyUrl;
			}
			if (typeof settings.popupPosition !== 'undefined') {
				_self.params.popupPosition = settings.popupPosition;
			}
			if (typeof settings.colorStyle !== 'undefined') {
				_self.params.colorStyle = settings.colorStyle;
			}
			if (typeof settings.popupTitle !== 'undefined') {
				_self.params.popupTitle = settings.popupTitle;
			}
			if (typeof settings.popupText !== 'undefined') {
				_self.params.popupText = settings.popupText;
			}
			if (typeof settings.buttonContinueTitle !== 'undefined') {
				_self.params.buttonContinueTitle = settings.buttonContinueTitle;
			}
			if (typeof settings.buttonLearnmoreTitle !== 'undefined') {
				_self.params.buttonLearnmoreTitle = settings.buttonLearnmoreTitle;
			}
			if (typeof settings.buttonLearnmoreOpenInNewWindow !== 'undefined') {
				_self.params.buttonLearnmoreOpenInNewWindow = settings.buttonLearnmoreOpenInNewWindow;
			}
			if (typeof settings.agreementExpiresInDays !== 'undefined') {
				_self.params.agreementExpiresInDays = settings.agreementExpiresInDays;
			}
			if (typeof settings.autoAcceptCookiePolicy !== 'undefined') {
				_self.params.autoAcceptCookiePolicy = settings.autoAcceptCookiePolicy;
			}
			if (typeof settings.htmlMarkup !== 'undefined') {
				_self.params.htmlMarkup = settings.htmlMarkup;
			}
		}

	};

	var createHtmlMarkup = function() {

		if (_self.params.htmlMarkup) {
			return _self.params.htmlMarkup;
		}

		var html =
			'<div class="cookiepopup-container' +
			    ' cookiepopup-container-' + _self.params.popupPosition +
			    (_self.params.compactStyle ? ' cookiepopup-style-compact' : '') +
				' cookiepopup-color-' + _self.params.colorStyle + '">' +
				'<div class="cookiepopup-head">' + _self.params.popupTitle + '</div>' +
				'<div class="cookiepopup-body">' + _self.params.popupText + '</div>' +
				'<div class="cookiepopup-buttons">' +
				  '<a href="' + _self.params.cookiePolicyUrl + '"' +
				 	(_self.params.buttonLearnmoreOpenInNewWindow ? ' target=_blank ' : '') +
					' class="cookiepopup-button cookiepopup-button_2">' + _self.params.buttonLearnmoreTitle + '</a>' +
				  '<a href="#" class="cookiepopup-button cookiepopup-button_1">' + _self.params.buttonContinueTitle + '</a>' +
				  '<div class="clearfix"></div>' +
				'</div>' +
				'<a href="#" class="cookiepopup-closebutton">x</a>' +
			'</div>';

		return html;
	};

	// Storing the consent in a cookie
	var setUserAcceptsCookies = function(consent) {
		var d = new Date();
		var expiresInDays = _self.params.agreementExpiresInDays * 24 * 60 * 60 * 1000;
		d.setTime( d.getTime() + expiresInDays );
		var expires = "expires=" + d.toGMTString();
		document.cookie = _self.vars.COOKIE_NAME + '=' + consent + "; " + expires + ";path=/";

		$(document).trigger("user_cookie_consent_changed", {'consent' : consent});
	};

	// Let's see if we have a consent cookie already
	var userAlreadyAcceptedCookies = function() {
		var userAcceptedCookies = false;
		var cookies = document.cookie.split(";");
		for (var i = 0; i < cookies.length; i++) {
			var c = cookies[i].trim();
			if (c.indexOf(_self.vars.COOKIE_NAME) !== -1) {
				userAcceptedCookies = c.substring(_self.vars.COOKIE_NAME.length + 1, c.length);
			}
		}

		return userAcceptedCookies;
	};

	var hideContainer = function() {
		// $('.cookiepopup-container').slideUp(200);
		$('.cookiepopup-container').animate({
			opacity: 0,
			height: 0
		}, 200, function() {
			$('.cookiepopup-container').hide(0);
		});
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// PUBLIC FUNCTIONS  //////////////////////////////////////////////////////////////////////////
	var publicfunc = {

		// INITIALIZE EU COOKIE LAW POPUP /////////////////////////////////////////////////////////
		init : function(settings) {

			parseParameters(
				$(".cookiepopup").first(),
				$(".cookiepopup-markup").html(),
				settings);

			// No need to display this if user already accepted the policy
			if (userAlreadyAcceptedCookies()) {
        $(document).trigger("user_cookie_already_accepted", {'consent': true});
				return;
			}

			// We should initialise only once
			if (_self.vars.INITIALISED) {
				return;
			}
			_self.vars.INITIALISED = true;

			// Markup and event listeners >>>
			_self.vars.HTML_MARKUP = createHtmlMarkup();

			if ($('.cookiepopup-block').length > 0) {
				$('.cookiepopup-block').append(_self.vars.HTML_MARKUP);
			} else {
				$('BODY').append(_self.vars.HTML_MARKUP);
			}

			$('.cookiepopup-button_1').click(function() {
				setUserAcceptsCookies(true);
				hideContainer();
				return false;
			});
			$('.cookiepopup-closebutton').click(function() {
				setUserAcceptsCookies(true);
				hideContainer();
				return false;
			});
			// ^^^ Markup and event listeners

			// Ready to start!
			$('.cookiepopup-container').show();

			// In case it's alright to just display the message once
			if (_self.params.autoAcceptCookiePolicy) {
				setUserAcceptsCookies(true);
			}

		}

	};

	return publicfunc;
});

$(document).ready( function() {
	if ($(".cookiepopup").length > 0) {
		$(document).euCookieLawPopup().init({
			'info' : 'YOU_CAN_ADD_MORE_SETTINGS_HERE',
			'popupTitle' : '',
			'popupText' : 'This website uses cookies for analytics, personalization, and other purposes. Click to learn more. By continuing to browse, you agree to our use of cookies as described in our Cookies Statement.'
		});
	}
});

$(document).bind("user_cookie_consent_changed", function(event, object) {
	console.log("User cookie consent changed: " + $(object).attr('consent') );
});

}(jQuery));
