<!DOCTYPE html>
<html>
	{include file="head-block.tpl"}
	<body>
		<p>&nbsp;</p>
		<div class="col-sm-6 col-sm-offset-3">
			<div class="well">
				<p>{$data.message}</p>
				<div class="text-center">
					{makelink mode="site" action="listing" value="キャンセル" class="btn btn-success"}
					&nbsp;&nbsp;&nbsp;
					{makelink mode="logout" action="go" value="ログアウト" class="btn btn-danger"}
				</div>
			</div>
		</div>
		<div class="clear"></div>
	</body>
</html>
