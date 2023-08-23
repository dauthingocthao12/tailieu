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
			<!-- モーダル・ダイアログ -->
			{include file='alert_modal.tpl'}
			<!-- モーダル・ダイアログ -->

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
			<h2 class="font-color"><i class="fa fa-times"></i> ユーザー削除</h2>
			<div class="base-bg contents-space">
				<p class="text-info">
ご利用ありがとうございました。 <br/>
会員情報を削除します。<br>
<br/>
お差し支えなければ、<br/>
退会される理由(複数選択可能) をお聞かせください。<br/>
</p>
				<form action="" method="post" name="form">

<label class=""><input name="enquete[0]" type="checkbox" value="内容がつまらない。"> 内容がつまらない。</label><br/>
<label class=""><input name="enquete[1]" type="checkbox" value="サイトが使いにくい。"> サイトが使いにくい。</label><br/>
<label class=""><input name="enquete[2]" type="checkbox" value="表示が分かりにくい。"> 表示が分かりにくい。</label><br/>
<label class=""><input name="enquete[3]" type="checkbox" value="表示が遅い。"> 表示が遅い。</label><br/>
<label class=""><input name="enquete[4]" type="checkbox" value="広告表示が邪魔だから。"> 広告表示が邪魔だから。</label><br/>
<label class=""><input name="enquete[5]" type="checkbox" value="サイトの更新が少ないため。"> サイトの更新が少ないため。</label><br/>
<label class=""><input name="enquete[6]" type="checkbox" value="メール配信を受信するのが面倒。"> メール配信を受信するのが面倒。</label><br/>
<label class=""><input name="enquete[7]" type="checkbox" value="メール配信機能が使いにくい。"> メール配信機能が使いにくい。</label><br/>
<label class=""><input name="enquete[8]" type="checkbox" value="星座占いに興味が無くなったから。"> 星座占いに興味が無くなったから。</label><br/>
<br/>
<p class="text-info">
その他、具体的にご記入頂ける場合は、以下にお願い致します。<br/>
<textarea class="form-control not-zoom" name="enqtext"></textarea><br/>
ご登録されていたメールアドレス:<br/>
&nbsp;<span style="color: #333333;">{$registed_mail}</span><br/>
<br/>
以上で宜しければ、[ユーザ削除] ボタンを押してください。<br/>
<br/>
</p>
					<div class="text-center">
						<input class="btn fontw-backb" type="button" value="ユーザー削除" onclick="input_madrs_check();return false;">
					</div>
<input type="hidden" name="email" value="{$registed_mail}">
<input type="hidden" name="num_enquete" value="10">
				</form>
			</div>
			{/if}

		</div>
	</div>
</div>

{/block}
