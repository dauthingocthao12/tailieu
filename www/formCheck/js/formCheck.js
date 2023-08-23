/**
@param fields_
@return void
*/
function formCheckIndicator(fields_) {
	for(var i=fields_.length-1; i>=0; --i) {
		var field = fields_[i];
		$('[name='+field+']').parents('.form-group:first').addClass('required');
	}
}


/**
displays errors for the fields in the list with the corresponding message
@param object {name: message}
@return void
*/
function formCheckErrors(fields_) {
	for(var field in fields_) {
		$('[name='+field+']').parents('.form-group:first').addClass('has-error');
		// TODO add message!
		var message = $('<div class="alert alert-danger animated bounce form-check">'+fields_[field]+'</div>');
		$('[name='+field+']').after(message);
	}
}
