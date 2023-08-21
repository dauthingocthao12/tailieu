<!DOCTYPE html>
<html>
	{include file="head-block.tpl"}
	<body>
		<p>&nbsp;</p>
		<div class="col-sm-6 col-sm-offset-3">
			<div class="well">
				<form action="" method="POST">
					{if not $smarty.session.user}
					{xmakelink mode="login" action="check" value="ログイン" class="btn btn-primary"}
					<div class="form-group">
						<label>ユーザ名</label>
						{xinput type="text" name="user" value="" class="form-control"}
					</div>
					<div class="form-group">
						<label>パスワード</label>
						{xinput type="password" name="pass" value="" class="form-control"}
					</div>
					{/xmakelink}
					{else}
					{makelink mode="logout" action="check" value="ログアウト"}
					{/if}
				</form>
				<p>&nbsp;</p>
				<p class="alert alert-info">
				{$data.message}
				</p>
			</div>
		</div>
		<div class="clear"></div>
	</body>
</html>

