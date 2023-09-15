/**
 * Fixes a navigation element at the top of the screen
 * when page is scrolled.
 * 
 * Requirements:
 *  - jQuery
 *
 * Usage:
 *  - FixedMenu.init("#nav-main", "header");
 *
 * Tested On:
 *  - IE11
 *  - Chrome 79
 *  - Edge (Win10 1903)
 *  - iOS 13 Safari
 */
(function($) {
FixedMenu = {
	/**
	 * @param nav_ string selector of the nav element to fix on scroll
	 * @param offset_ string : selector of the element to offset body when fixing nav
	 */
	init: function(nav_, offset_) {
		var nav = $(nav_);
		var header = $(offset_);

		// attach event
		$(window).scroll(function() {
			if($(window).scrollTop() > header.height()) {
				nav.css({
					position: 'fixed',
					top: 0,
					left: 0,
					right: 0,
					zIndex: 99
				});
				$("body").css('padding-top', nav.height()+"px");
			}
			else if($("body").hasClass("mobile-menu-open")==false) {
				nav.css('position', 'static');
				$("body").css('padding-top', 0);
			}
		});
	},
};
})(jQuery);