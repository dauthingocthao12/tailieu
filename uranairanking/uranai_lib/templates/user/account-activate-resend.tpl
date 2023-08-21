{extends file="main.tpl"}
{block name=seo}
<title>サイトについて 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp">
<meta name="description" content="サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。">
{/block}
{block name=body}

<div class="account container">
	<div class="page-center clearfix">
		<div class="col-sm-6 col-sm-offset-3">

			<p><a class="btn btn-primary" href="{sitelink mode="account/login"}"><i class="fa fa-arrow-left"></i> ログイン画面に戻る</a></p>

			{if $mailError}
			<div class="alert alert-danger">
				メールアドレスが見つかりませんでした。
			</div>
			{/if}

			{if $mailSendERR}
			<div class="alert alert-danger">
				メール送信エラーです。
			</div>
			{/if}

			{if $mailSendOK}
			<div class="alert alert-success">
				アクティベーションのメールを送りました。
			</div>
			{/if}

			<div class="well">
				<h2><i class="fa fa-envelope"></i> 確認メールの再送信</h2>
				<p class="text-info">
				登録されたメールアドレスへ、メールアドレスの確認用のメールを送ります。<br>
				<br/>
				下記に登録したメールアドレスを入力して [送信] ボタンを押してください。<br/>
				もしメールが届かない場合には、迷惑メールフォルダなどもご確認ください。<br/>
				</p>
				<form action="" method="post">
					<div class="form-group">
						<label for="email">メールアドレス</label>
						<input type="text" class="form-control" name="email" id="email">
						<p class="text-info">
						※登録した時のメールアドレス以外には送信できません。
						</p>
					</div>
					<div class="text-center">
						<input class="btn btn-success" type="submit">
					</div>
				</form>
			</div>

		</div>
	</div>
</div>

{/block}
