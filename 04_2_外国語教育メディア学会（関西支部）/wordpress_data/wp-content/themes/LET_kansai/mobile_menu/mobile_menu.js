// main side menu (mobile)
// to set in a ready event
// depends on:
//    - jQuery


(function($) {
	function copySubMenu()
	{
		var mm = $("#mobile-menu");
		// if needed, copy side menu
		if(mm.html()=="") {
			var mainmenu = $("#main-menu-list").clone();
			mm.append(mainmenu);
			var sidemenu = $("#side-menu-content").clone();
			mm.append(sidemenu);
		}
		// reset its scrolling position
		mm.scrollTop(0);
	}

	$(document).ready(function() {
		var scrollOffset;

		// OPEN button + CLOSE button
		$(".mobile-menu-close-btn, .mobile-menu-open-btn").click(function(e_) {
			e_.preventDefault();
			scrollOffset = $(window).scrollTop();
			$("body").toggleClass("mobile-menu-open");
			if($("body").hasClass("mobile-menu-open")) {
				// menu opened
				$("body").css("top", (-1 * scrollOffset) + "px");
				// copy submenu
				copySubMenu();
			}
			else {
				// menu closed
				scrollOffset = (-1) * ($("body").css("top").replace("px", ""));
				$(window).scrollTop(scrollOffset);
			}
		});
	});
})(jQuery);