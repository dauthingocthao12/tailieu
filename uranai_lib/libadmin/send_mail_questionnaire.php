<?php
function mail_kakunin(){
	
	$result =  '<div class="well">';
	$result .=  mail_content();
	$result .=  '</div>';
	$result .=  '送信するをクリックすると、登録するユーザーさんにメールが送信されます。<br>';
	$result .=  '上の内容でメールを送りますがよろしいですか？';
	
	$result .=  '<FORM action="index.php" method="post">';
	$result .=  '<input type="hidden" name="mode" value="send_mail">';
	$result .=  '<input type="hidden" name="action" value="send">';
	$result .=  '<input type="submit" value="送信する" onclick="return confirm(\'本当に送ってよろしいですか？\')";>';
	$result .=  '</FORM>';
	
	$return_array['data'] = $result;
	$return_array['message'] = "";
	$return_array['title'] = "メール送信";
	return $return_array;
	
}

function send_mail_questionnaire() {
	//アクティブサイト一覧の前前日から前日までのデータを出す。(まだ取得できていないサイトもチェックできるように)
	global $conn;
	
	$sql = "SELECT *";
	$sql .= " FROM  `users` ";
	$sql .= " WHERE (notification1 = 1";
	$sql .= " OR notification1 = 1";
	$sql .= " OR notification2 = 1";
	$sql .= " OR notification3 = 1";
	$sql .= " OR notification4 = 1";
	$sql .= " OR notification5 = 1";
	$sql .= " OR notification6 = 1";
	$sql .= " OR notification0 = 1";
	$sql .= " OR notificationSW = 1)";
	$sql .= " AND user_id NOT IN (119,27)";//除外したいメールリスト//以前お問い合わせメールをいただいた方に注意！本番用
//	$sql .= " AND user_id IN (87)";//送りたいメールリスト
	$sql .= " AND is_delete = 0";//除外したいメールリスト
	

	$rs = $conn->query($sql);
	$error = "";
	if($rs) {
		while($row = $rs->fetch_assoc()) {
			
			$mail_body = mail_content($row['handlename']);
			$from_adrs = $row['email'];

			$mail = new mail();
			$mail->set_encoding("utf-8");
			$ok = $mail->send(
				MAIL_SENDER_EMAIL,//送信元アドレス
				MAIL_SENDER_NAME,
				$from_adrs,
				'12星座占いランキングアンケートのお願い',
				$mail_body);
			
			
			if(!$ok){
				$error = $row['user_id'].',';
			}
			
		}
	}else{
		$result =  $sql;
	}
	if($error){
		$send_result =  $error;
	}else{
		$send_result =  'OK';
	}
	$result .=  '<FORM action="index.php" method="post" name="send_result">';
	$result .=  '<input type="hidden" name="mode" value="send_mail">';
	$result .=  '<input type="hidden" name="action" value="send_result">';
	$result .=  '<input type="hidden" name="error_id" value="'.$send_result.'">';
	$result .=  '</FORM>';
	$result .=  '<script>';
	$result .=  'document.send_result.submit()';
	$result .=  '</script>';
	
	$return_array['data'] = $result;
	$return_array['message'] = "";
	$return_array['title'] = "メール送信";
	return $return_array;

}
function mail_result(){
	$result = "";
	if(isset($_POST['error_id'])){
		$id = $_POST['error_id'];
		if($id == 'OK'){
			$result = '全て送信しました';
		}else{
			$id = rtrim($id, ',');
			$id_list = explode(",",$id);
			foreach ($id_list as $value) {
				$result .= 'user_id'.$value.'の送信に失敗しました<br>';
			}
		}
	}else{
		$result = '不明です。実際にメールを確認してください';
	}
	$return_array['data'] = $result;
	$return_array['message'] = "";
	$return_array['title'] = "メール送信結果";
	return $return_array;
}
function mail_content($name = NULL){
	if(!$name){
		$name = "ななし";
	}
$massage = <<<EOM
{$name} 様

日頃より12星座占いランキングをご利用いただき、誠にありがとうございます。

この度、ユーザー様により良いサービスを提供させていただくため、
アンケートを実施させていただいております。

{$name}様には、お手数をおかけしてしまい申し訳ありませんが、
アンケートは、全８問の簡単なものとなっておりますので、
貴重なご意見をお聞かせいただけますと幸いです。

アンケートURL
https://forms.gle/B3BfiAvnjjt1RZtz8

実施期間
2019/05/22 ~ 2019/07/31

12星座占いランキング
https://uranairanking.jp/
EOM;
	
	return $massage;
}

