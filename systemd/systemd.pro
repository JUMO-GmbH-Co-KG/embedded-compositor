TEMPLATE=aux

systemd.files = compositor.service
systemd.path = /lib/systemd/system

environments.files = env-compositor env-clients
environments.path = /etc/default/compositor

INSTALLS = systemd environments
