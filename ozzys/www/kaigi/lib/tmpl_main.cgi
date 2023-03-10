

<!--■ メール通知テンプレートファイル -->




<!--▼メールヘッダブロック-->
<!--BLOCK="mail_header"-->
Subject: /?mail_subj?/
To: /?mail_addr?/
From: /?mail_addr?/
X-Priority: /?mail_priority?/
X-Mailer: CGI_Mailer
aContent-Type: text/plain; charset="UTF-8"

<!--/BLOCK="mail_header"-->




<!--▼メールボディブロック-->
<!--BLOCK="mail_body"-->
「/?title?/」に新しい投稿がありました。

題名　　　　: /?subj?/
名前　　　　: /?name?/
メール　　　: </?mail?/>
ＵＲＬ　　　: </?url?/>
投稿者ホスト: /?host?/
投稿日時　　: /?date?/

メッセージ　:

/?msg?/

<!--/BLOCK="mail_body"-->




