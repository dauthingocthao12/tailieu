<?php

class mail {
	var $language = "ja";
	var $encoding = "Shift_JIS";
	var $returnPath = '';

	function set_language($val) {
		$this->language = $val;
	}
	function set_encoding($val) {
		$this->encoding = $val;
	}

	/**
	 * メールヘッダー設定：Return-Path
	 * メールが届かない場合はREturn-Pathに戻ります
	 * 
	 * @param string $path_ メールアドレス
	 */
	function setReturnPath($path_) {
		$this->returnPath = $path_;
	}

	function send($from_mail,$from_name,$to_mail,$subject,$msg,$cc_mail='',$bcc_mail='') {
		if ($this->encoding == "utf-8") { mb_language(uni); }
		else { mb_language($this->language); }
		mb_internal_encoding($this->encoding);

		$from_name = mb_encode_mimeheader($from_name);
		
		// headers
		$additional_headers = "From: $from_name <$from_mail>\n";
		$additional_headers .= "Reply-To: $from_mail\n";
		if($this->returnPath) {
			$additional_headers .= "Return-Path: {$this->returnPath}";
		}
		if ($cc_mail) { $additional_headers .= "Cc: $cc_mail\n"; }
		if ($bcc_mail) { $additional_headers .= "Bcc: $bcc_mail\n"; }

		if (!mb_send_mail ($to_mail,$subject,$msg,$additional_headers , "-f$from_mail")) {
			return false;
		}

		return true;
	}
}
