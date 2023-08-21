{extends file="main.tpl"}
{block name=seo}
<title>パスワードをお忘れの場合 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,パスワードをお忘れの場合">
<meta name="description" content="12星座占いサイトを独自に集計しランキングを出しています。パスワードをお忘れの場合">
{/block}
{block name=body}

<div class="account container">
	<div class="page-center clearfix">
		<div class="col-sm-6 col-sm-offset-3">

			<p><a class="btn btn-primary" href="{sitelink mode="account/login"}"><i class="fa fa-arrow-left"></i> ログイン画面に戻る</a></p>

			{if $mailError}
			<div class="alert alert-danger">
				入力されたメールアドレスは登録されていません。
			</div>
			{/if}

			{if $mailSendERR}
			<div class="alert alert-danger">
				メール通信がエラーになりました。
			</div>
			{/if}

			{if $mailSendOK}
			<div class="alert alert-success">
				メールを送信しました。
			</div>
			{/if}
			<div class="alert alert-danger mail_null" style="display:none;">
				メールアドレスが未入力です。
			</div>
			<div class="alert alert-danger mail_miss" style="display:none;">
				メールアドレスの形式が正しくありません。
			</div>
			<!-- モーダル・ダイアログ -->
			{include file='alert_modal.tpl'}
			<!-- モーダル・ダイアログ -->
				<h2 class="font-color"><i class="fa fa-key"></i> パスワードをお忘れの場合</h2>
			<div class="base-bg contents-space">
				<form action="" method="post" name="form">
					<div class="form-group">
						<label for="email">メールアドレス</label>
						<input type="text" class="form-control not-zoom" name="email" id="email">
						<p class="text-info">
						登録しているメールアドレスを入力して、[パスワード再発行] のボタンを押して下さい。<br/>
						このメールアドレスへ、再設定したパスワードをお知らせするメールが届きます。<br/>
						</p>
					</div>
					<div class="text-center">
						<input class="btn fontw-backb" type="button" value="パスワード再発行" onclick="input_madrs_check(1);return false;">
					</div>
				</form>
			</div>

		</div>
	</div>
</div>

{/block}

