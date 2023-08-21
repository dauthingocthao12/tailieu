{extends file="main.tpl"}
{block name=seo}
<title>ユーザー削除 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,ユーザー削除">
<meta name="description" content="12星座占いサイトを独自に集計しランキングを出しています。ユーザー削除を行ないます。">
{/block}
{block name=body}

<div class="account container">
	<div class="page-center clearfix">
		<div class="col-sm-6 col-sm-offset-3">

			<p><a class="btn btn-primary" href="/"><i class="fa fa-arrow-left"></i> トップに戻る</a></p>

			{if $mailError}
			<div class="alert alert-danger">
				メールアドレスが見つかりませんでした。
			</div>
			{/if}

			{if $deleteOK}
			<div class="alert alert-success">
				アカウントを削除しました。
			</div>
			{else}
			<div class="font-color tecen">
				<h2><i class="fa fa-times"></i> ユーザー削除</h2>
			</div>
			<div class="base-bg contents-space">
				<p class="text-info">
				"12星座占いランキング" から、会員情報を削除します。<br>
				下記に登録されているメールアドレスを入力して [ユーザー削除] ボタンを押してください。<br/>
				</p>
				<form action="" method="post" name="form">
					<div class="form-group">
						<label for="email">メールアドレス</label>
						<input type="text" class="form-control" name="email" id="email">
						<p class="text-info">
						<!--登録されているメールアドレスを入力して下さい。<br/>-->
						</p>
					</div>
					<div class="text-center">
						<input class="btn btn-success" type="button" value="ユーザー削除" onclick="input_madrs_check();return false;">
					</div>
				</form>
			</div>
			{/if}

		</div>
	</div>
</div>

{/block}
