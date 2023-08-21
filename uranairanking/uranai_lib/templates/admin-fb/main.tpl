<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Social-media Posting Admin Page</title>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous" />
	<style>
		.navbar {
			border-radius: 0;
		}
	</style>
</head>
<body>

	<nav class="navbar navbar-inverse">
		<div class="container">
			<div class="navbar-header">
				<a class="navbar-brand" href="index.php">12星座占いランキング.jp</a>
			</div>
			<div id="navbar" class="collapse navbar-collapse">
				<ul class="nav navbar-nav">
					<li><a href="?action=post-facebook">Post to Facebook</a></li>
					<li><a href="?action=logout">Logout</a></li>
				</ul>
			</div><!--/.nav-collapse -->
		</div>
	</nav>

	<div class="container">

		{block name="main"}{/block}
	</div>

</body>
</html>
