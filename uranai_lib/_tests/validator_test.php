<?php
//CommonValidatorクラスのテスト
require_once __DIR__ . '/../libadmin/common_validator.class.php';

//入力
$validator = new CommonValidator([
	'name' => 'yamada',
	// 'email' => 'aaa@bbb.jp',
	'email' => '',
	'furigana' => 'フリガナ',
	'comment' => '',
	'foo' => 'aaaaa',
]);
$validator->val('xyz', 123);
// $validator->val('abc', 'ABC');
$validator->val('abc', 500);

//
$validator->required('name', '名前は入力必須です。')
	->required('furigana', 'フリガナは入力必須です。')
	->required('email', 'メールアドレスは入力必須です。')
	->required('comment', 'お問い合わせ内容は入力必須です。')
	->required('foo', '')
	->rule('abc', '数値ではないようです!', 'is_numeric')
	->rule('email', 'メールアドレスが不正です。', 'CommonValidator::isEmailAdress')
	->rule('furigana', 'フリガナは全角カタカナで入力してください。', 'CommonValidator::isZenkakuKatakana')
	->rule('comment', 'メールアドレスじゃない!', 'CommonValidator::isEmailAdress')
	->rule('comment', '長すぎ！', 'CommonValidator::lengthMax', 10)
	->regex('foo', 'アルファベットじゃない!', '/^[A-Za-z]+$/');
if (!$validator->validate()) {
	print_r($validator->getErrors());
}
