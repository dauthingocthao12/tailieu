$('document').ready(function() {
	// bootstrap
	// responsive (menus)
	Responsive.init();

	// back to top arrow
	TopArrow.init();

	// change side bar mobile botton if on front page! >>>
	//console.log(location.pathname);
	if(location.pathname == "/") {
		$("#mobile-button-right").html( "おすすめ情報" );
	}
	else {
		$("#mobile-button-right").html( "お知らせ" );
	}
	// <<<
	
	// temp >>>
	/*
	var keywords = $('meta[name=keywords]').attr('content');
	var description = $('meta[name=description]').attr('content');
	var seodebug = $('<div></div>');
	seodebug.css({
		'position': 'fixed',
		'top': '10px',
		'left': '5px',
		'border': '1px solid red',
		'padding': '5px',
		'background' : 'white',
		'font-family' : 'sans'
	});
	seodebug.append("<p><b>keywords</b>:    " + keywords + "</p>");
	seodebug.append("<p><b>description</b>: " + description + "</p>");
	$('body').append(seodebug);
	*/
	// <<<
});

// Responsive logic >>>
Responsive = {
	winWidth: 0,
	previousMode: '', // 値： pc, mobile
	previousMenuLeft: false,
	previousMenuRight: false,

	leftMenuTrigger: 949, // 949px以下左モバイルメニューが表示
	rightMenuTrigger: 779, // 779px以下右モバイルメニューが表示

	init: function() {
		// window resize event
		$(window).resize(Responsive.handle);

		// size init
		this.handle();

		// display header navigation bar
		$('#navigation-header').show();

		// mobile events (menus)
		$('.mobile-menu .mobile-button').click(this.mobileMenuToggle);
		$('#mobile-menu-overlay').click(this.mobileMenuClose);
	},

	viewport: function() {
		// exception
		/*
		if(/iPad/.test(navigator.userAgent)) {
			return { width: document.width, height: document.height};
		}
		*/

		var e = window, a = 'inner';
		if (!('innerWidth' in window )) {
			a = 'client';
			e = document.documentElement || document.body;
		}
		return { width : e[ a+'Width' ] , height : e[ a+'Height' ] };
	},

	// jquery event
	handle: function() {
		var winWidth = Responsive.viewport().width;
		if(winWidth == Responsive.winWidth) {
			return;
		}
		else {
			Responsive.winWidth = winWidth;
		}

		var currentMode = 'pc',
			mobileMenuLeft = false,
			mobileMenuRight = false;

		// breakpoint
		if(winWidth<=Responsive.rightMenuTrigger) {
			currentMode = 'mobile';
			mobileMenuRight = true;
		}

		if(winWidth<=Responsive.leftMenuTrigger) {
			currentMode = 'mobile';
			mobileMenuLeft = true;
		}

		// mode change
		if(Responsive.previousMode != currentMode
			|| Responsive.previousMenuLeft != mobileMenuLeft
			|| Responsive.previousMenuRight != mobileMenuRight
		) {

			if(currentMode=='mobile') {
				// move navigation-header
				$('#navigation-mobile').append( $('#navigation-header') );

				if(mobileMenuLeft) {
					// change to Mobile mode
					// ---------------------

					// mobile button
					$('#mobile-button-left').show();

					// move login buttons
					$('#mobile-login-menu').append( $('#login-menu') );
					// move left menu
					$('#mobile-navbox').append( $('#navbox') );
				}

				if(mobileMenuRight) {
					// change to Mobile mode
					// ---------------------

					// mobile button
					$('#mobile-button-right').show();

					// move right menu
					$('#mobile-col-right').append( $('#col-right') );
				}

				$('body').removeClass('pc').addClass('mobile');
			}
			else {
				// PC mode
				// -------

				// move navigation-header
				$('#navigation-pc').append( $('#navigation-header') );
				
				// pc
				$('body').removeClass('mobile').addClass('pc');
			}

			// right menu restore
			if( (currentMode=='mobile' && mobileMenuRight==false)
				|| currentMode=='pc'
			) {

				// hide mobile menu button
				$('#mobile-button-right').hide();
				// move right menu
				$('#pc-col-right').append( $('#col-right') );
			}

			if( (currentMode=='mobile' && mobileMenuLeft==false)
				|| currentMode=='pc'
			) {

				// hide mobile menu button
				$('#mobile-button-left').hide();
				// move login buttons
				$('#navigation-header').append( $('#login-menu') );
				// move left menu
				$('#pc-col-left').append( $('#navbox') );
			}

			Responsive.mobileMenuClose();
			Responsive.previousMode = currentMode;
		}
	}, // end handle()


	// jquery event
	mobileMenuToggle: function() {
		if( $('#mobile-menu-content').is(':visible') ) {
			Responsive.mobileMenuClose.call(this);
		}
		else {
			Responsive.mobileMenuOpen.call(this);
		}
	},

	// jquery event
	mobileMenuOpen: function() {
		$(this).siblings('.mobile-content').show();
		$('#mobile-menu-overlay').show();
		$('body').addClass('mobile-menu-opened');
	},

	// jquery event
	mobileMenuClose: function() {
		$('.mobile-content').hide();
		$('#mobile-menu-overlay').hide();
		$('body').removeClass('mobile-menu-opened');
	}

};
// <<<

// Top Arrow logic >>>
/**
* top arrow (scrool to top)
* depends on:
* - jQuery
* The button to be displayed is #TopArrowScrollBtn
* CSS sample for the button:
* #TopArrowScrollBtn {
* 	position: fixed;
* 	right: 10px;
* 	bottom: 10px;
* 	padding: 10px;
* 	background-color: rgba(0, 0, 0, 0.8);
* 	border-radius: 5px;
* 	color: white;
* 	font-weight: bold;
* 	text-decoration: none;
* }
* usage: 
* - TopArrow.init();
* tested on:
* - IE 9,11
* - Firefox 49
* - Edge
* - PC Chrome 53
* - iPad (iOS 9)
* - Android 7 Chrome
*/
var TopArrow = {
	init: function() {
		// arrow itself
		var topArrow = jQuery('<a id="TopArrowScrollBtn" style="display: none;" href="#scroll"><img src="/images/btn-toparrow.png" /></a>');
		// button event
		topArrow.click(this.doScroll);

		// appending element to the page
		jQuery('body').append(topArrow);

		// scroll event handler
		jQuery(document).scroll(this.checkScroll);
	},


	// jQuery event
	doScroll: function(e_) {
		e_.preventDefault();
		// possible to have different buttons with different actions
		if(jQuery(this).attr('href')==='#scroll') {
			//var offset = (jQuery(document).scrollTop());
			//if(/(IE 9|Trident)/.test(navigator.userAgent)) {
			//	// IE 9,11 only
			//	jQuery('body, html').animate({scrollTop: '-'+offset+'px'});
			//}
			//else if(/Firefox/.test(navigator.userAgent)) {
			//	jQuery('body, html').animate({scrollTop: 0});
			//} else {
			//	// others
			//	//jQuery('body').animate({scrollTop: '-'+offset+'px'});
			//}
			jQuery('body, html').animate({scrollTop: 0});
		}
	},


	// scroll check function
	// jQuery event
	checkScroll: function () {
		var topscroll = jQuery(document).scrollTop();
		if(topscroll>0) {
			// show
			jQuery('#TopArrowScrollBtn').fadeIn('fast');
		}
		else {
			// hide
			jQuery('#TopArrowScrollBtn').fadeOut('fast');
		}
	}
};
// <<<

