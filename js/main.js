// Modified from Beautiful Jekyll by Dean Attali
// vim:set ts=4 sw=4:
var main = {
	init : function() {
		// Minimise navbar after scrolling a little bit down
		$(window).scroll(function() {
			if ($(".navbar-fixed-top").offset().top > 50) {
				$(".navbar-fixed-top").addClass("navbar-minimised");
			} else {
				$(".navbar-fixed-top").removeClass("navbar-minimised");
			}
		});

		// On mobile, hide the avatar when expanding the navbar menu
		$('#main-navbar').on('show.bs.collapse', function () {
			$(".navbar-fixed-top").addClass("navbar-expanded");
		});
		$('#main-navbar').on('hidden.bs.collapse', function () {
			$(".navbar-fixed-top").removeClass("navbar-expanded");
		});
	}
};

document.addEventListener('DOMContentLoaded', main.init);
