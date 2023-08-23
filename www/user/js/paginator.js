$(document).ready(function() {
	$('.navigation-paginator li a').click(function(e_) {
		e_.preventDefault();
		var page = $(this).attr('href').split('/')[1];

		// send form
		var form = $('#form-paginator');
		$('[name=page]', form).val(page);
		form.submit();

	});
});
