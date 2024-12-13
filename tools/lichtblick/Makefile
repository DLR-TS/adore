
.EXPORT_ALL_VARIABLES:
DOCKER_CONFIG:=

.PHONY: build
build:
	docker build . -t lichtblick:latest -f Dockerfile.lichtblick

.PHONY: clean
clean:
	docker rmi --force lichtblick:latest > /dev/null 2>&1 || true

.PHONY: start
start:
	@echo "Starting Lichtblick..."
	docker stop -f lichtblick > /dev/null 2>&1 || true
	docker rm -f lichtblick > /dev/null 2>&1 || true
	docker run -d --rm --network host --name lichtblick lichtblick:latest
	@echo "Lichtblick started, navigate to http://localhost:8080 for the web ui"

.PHONY: stop
stop:
	@echo "Stoping Lichtblick..."
	docker stop -f lichtblick > /dev/null 2>&1 || true
	docker rm -f lichtblick > /dev/null 2>&1 || true
	@echo "Lichtblick stopped"

