<?php

require_once __DIR__ . "/../libadmin/common_validator.class.php";

class ContactFormHandler {

    private $userName;
    private $furigana;
    private $tel;
    private $email;
    private $confirm_email;
    private $comment;
    private $validator = null; //バリデーション用オブジェクト 
    private $token;

    function __construct() {
        $this->validator = new CommonValidator;
    }

    //setter
    function setUserName($name) {
        $this->userName = $name;
        $this->validator
            ->val('name', $name)
            ->required('name', 'お名前は入力必須です。');
        return $this;
    }

    function setFrigana($furigana) {
        $this->furigana = $furigana;
        $this->validator
            ->val('furigana', $furigana)
            ->required('furigana', 'フリガナは入力必須です。')
            ->rule('furigana', 'フリガナは全角カタカナで入力してください。', 'CommonValidator::isZenkakuKatakana');
        return $this;
    }

    function setTel($tel) {
        $this->tel = $tel;
        return $this;
    }

    function setEmail($email) {
        $this->email = $email;
        $this->validator
            ->val('email', $email)
            ->required('email', 'メールアドレスは入力必須です。')
            ->rule('email', 'メールアドレスが不正です。', 'CommonValidator::isEmailAdress');
        return $this;
    }

    function setConfirmEmail($confirm_email) {
        $this->confirm_email = $confirm_email;

        $email = $this->email;
        $this->validator
            ->val('confirm_email', $confirm_email)
            ->required('confirm_email', '確認用メールアドレスは入力必須です。')
            ->rule('confirm_email', '確認用メールアドレスが、メールアドレスと一致しません。', function() use ($email, $confirm_email) {
                return $email == $confirm_email;
            });
        return $this;
    }

    function setComment($comment) {
        $this->comment = $comment;
        $this->validator
            ->val('comments', $comment)
            ->required('comments', 'お問い合わせ内容は入力必須です。');
        return $this;
    }

    function validate() {
        $this->validator->validate();
        return $this->validator->getErrors();
    }

    //管理者にメールをおくる
    function sendEmailToAdmin() {
        
        $body  = '';
        $body .= 'お名前：' . $this->userName . PHP_EOL;
        $body .= 'フリガナ:' .$this->furigana . PHP_EOL;
        $body .= '電話番号:' .$this->tel . PHP_EOL;
        $body .= 'メールアドレス:' .$this->email . PHP_EOL;
        $body .= '内容:' .$this->comment . PHP_EOL;

        $mail = new mail();
		$mail->set_encoding("utf-8");
		$ok = $mail->send(
			MAIL_SENDER_EMAIL,
			MAIL_SENDER_NAME,
            'info@uranairanking.jp',
			'お問い合わせ',
			$body
        );
		return $ok;
    }
        //利用者さんにメールをおくる
        function sendEmailToUser() {
        
            $body  = '';
            $body .= $this->userName.' 様'. PHP_EOL;
            $body .= ''. PHP_EOL;
            $body .= 'お問い合わせいただき、ありがとうございました。'. PHP_EOL;
            $body .= '以下の内容でお問い合わせを受け付けいたしました。'. PHP_EOL;
            $body .= '※このメールは自動返信によって送信されています。'. PHP_EOL;
            $body .= 'もしお心当たりのない場合、本メールは破棄して頂けるようお願いいたします。'. PHP_EOL;
            $body .= ''. PHP_EOL;
            $body .= '【お問い合わせ内容】'. PHP_EOL;
            $body .= 'お名前：' . $this->userName . PHP_EOL;
            $body .= 'フリガナ:' .$this->furigana . PHP_EOL;
            $body .= '電話番号:' .$this->tel . PHP_EOL;
            $body .= 'メールアドレス:' .$this->email . PHP_EOL;
            $body .= '内容:' .$this->comment . PHP_EOL;
            $body .= ''. PHP_EOL;
            $body .= ''. PHP_EOL;
            $body .= '__________________________________'. PHP_EOL;
            $body .= '12星座占いランキング'. PHP_EOL;
            $body .= 'URL: https://uranairanking.jp/'. PHP_EOL;
            $body .= 'Email: info@uranairanking.jp'. PHP_EOL;
            $body .= '__________________________________'. PHP_EOL;


    
            $mail = new mail();
            $mail->set_encoding("utf-8");
            $ok = $mail->send(
                MAIL_SENDER_EMAIL,
                MAIL_SENDER_NAME,
                $this->email,
                '[12星座占いランキング]お問い合わせありがとうございます',
                $body
            );
            return $ok;
        }

    //トークン発行
    function generateToken(){
        // https://qiita.com/ucan-lab/items/06e2ae043fbba4d72843より引用
        $this->token = uniqid(bin2hex(random_bytes(1)));
        return $this->token;
    }
    // セッションにトークンを保存する
    function saveTokenToSession(){
        $_SESSION['contactFormToken'] = self::generateToken();
    }
    function getToken(){
        return $this->token;
    }
    function getTokenFromSession(){
        return $_SESSION['contactFormToken'];
    }
    function deleteToken(){
        unset($_SESSION['contactFormToken']);
    }

}

