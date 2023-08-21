<!DOCTYPE html>
<html>
	{include file="head-block.tpl"}
	<body>
		<div class="container">
			<h1>管理</h1>
			{if $data.message}
			<div class="alert alert-info">{$data.message}</div>
			{/if}
			{makelink mode="log" action="listing" value="&#xf02d; ログ" class="btn-fa btn btn-primary"}
			{makelink mode="batch-job-status" action="listing" value="&#xf02d; 取得状況" class="btn-fa btn btn-primary"}
			{makelink mode="site" action="listing" value="&#xf0ac; サイト一覧" class="btn-fa btn btn-primary"}
			{makelink mode="ad" action="listing" value="&#xf1ea; 広告一覧" class="btn-fa btn btn-primary"}
			{makelink user_sort_column="email" user_order="ASC" mode="user" action="listing" value="&#xf007; ユーザ一覧" class="btn-fa btn btn-primary"}
			{makelink mode="news" action="listing" value="&#xf09e; 新着一覧" class="btn-fa btn btn-primary"}
			{makelink mode="comment" action="listing" value="&#xf27a; コメント管理" class="btn-fa btn btn-primary"}
			{makelink mode="site_check" action="listing" value="&#xf27a; サイトチェック確認ツール" class="btn-fa btn btn-primary"}
			{makelink mode="analysis" action="listing" value="アクセス解析" class="btn-fa btn btn-primary"}
			{makelink mode="sougolink" action="listing" value="相互リンク管理" class="btn-fa btn btn-primary"}
			<!--{makelink mode="send_mail" action="listing" value="&#xf27a; メール送信ツール" class="btn-fa btn btn-primary"}-->
			{makelink mode="logout" action="check" value="ログアウト" class="pull-right btn btn-danger"}
			<hr>
			{block name=body}{/block}
		</div>
	</body>
</html>
